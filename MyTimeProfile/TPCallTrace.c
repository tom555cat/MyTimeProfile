//
//  TPCallTrace.c
//  MyTimeProfile
//
//  Created by tongleiming on 2021/7/1.
//

#include "TPCallTrace.h"
#include <pthread/pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "fishhook.h"
#include <objc/runtime.h>


// 局部变量，使用的函数指针
void (*orgin_objc_msgSend)(void);

static pthread_key_t threadKeyLR;

#warning 无限参数的函数怎么表示?
#warning objc_msgSend不是无限参数吗
extern void hook_msgSend();

typedef struct {
    int allocLength;
    int index;
    uintptr_t *lr_stack;
} LRStack;

static struct LRStack *lrStack;

void hookObjcMsgSend()
{
    struct rebinding rebindingObjcMsgSend;
    rebindingObjcMsgSend.name = "objc_msgSend";
    rebindingObjcMsgSend.replacement = hook_msgSend;        // 替换成hook_msgSend函数
    rebindingObjcMsgSend.replaced = (void *)&orgin_objc_msgSend;    // 保存原始的objc_msgSend函数调用
    
    struct rebinding rebs[1] = {rebindingObjcMsgSend};
    rebind_symbols(rebs, 1);
}



// 为什么用内联函数
static inline void setLRRegisterValue(uintptr_t lr)
{
    // 每个线程独占一个lrStack
    LRStack *lrStack = pthread_getspecific(threadKeyLR);
    if (!lrStack) {
        lrStack = (LRStack *)malloc(sizeof(LRStack));
        lrStack->allocLength = 128;
        lrStack->lr_stack = (uintptr_t *)malloc(lrStack->allocLength * sizeof(uintptr_t));
        lrStack->index = -1;
        pthread_setspecific(threadKeyLR, lrStack);
    }
    if (++lrStack->index >= lrStack->allocLength) {
        lrStack->allocLength += 128;
        lrStack->lr_stack = (uintptr_t *)realloc(lrStack->lr_stack, lrStack->allocLength *sizeof(uintptr_t));
    }
    lrStack->lr_stack[lrStack->index] = lr;
}

static inline uintptr_t getLRRegisterValue()
{
    LRStack *lrStack = pthread_getspecific(threadKeyLR);
    if (lrStack->index > lrStack->allocLength) {
        return 0;
    }
    uintptr_t lr = lrStack->lr_stack[lrStack->index--];
    return lr;
}

void hook_objc_msgSend_before(id self, SEL selector, uintptr_t lr)
{
    // 一些记录操作
    printf("%s %s", object_getClassName(self), sel_getName(selector));
    
    // 记录LR寄存器
    setLRRegisterValue(lr);
}

uintptr_t hook_objc_msgSend_after(BOOL is_objc_msgSendSuper)   // 为什么这里要判断super
{
    // 一些操作
    
    return getLRRegisterValue();
}
