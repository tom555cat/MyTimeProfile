//
//  TPCallTrace.c
//  MyTimeProfile
//
//  Created by tongleiming on 2021/7/1.
//

#include "TPCallTrace.h"
#include <pthread/pthread.h>
#include <dispatch/dispatch.h>
#include <stdio.h>
#include <stdlib.h>
#include "fishhook.h"
#include <objc/runtime.h>


// 局部变量，使用的函数指针
void (*orgin_objc_msgSend)(void);
void (*orgin_objc_msgSendSuper2)(void);

static pthread_key_t threadKeyLR;

static bool CallRecordEnable = YES;

extern void hook_msgSend(void);

extern void hook_msgSendSuper2(void);

typedef struct {
    int allocLength;
    int index;
    uintptr_t *lr_stack;
} LRStack;

void threadCleanLRStack(void *ptr)
{
    if (ptr != NULL) {
        LRStack *lrStack = (LRStack *)ptr;
        if (lrStack->lr_stack) {
            free(lrStack->lr_stack);
        }
        free(lrStack);
    }
}

void startTrace(char *featureName)
{
    CallRecordEnable = YES;
    
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        // 初始化线程相关threadKeyLR，绑定析构函数
        pthread_key_create(&threadKeyLR, threadCleanLRStack);
        
        // hook objc_msgSend
        struct rebinding rebindingObjcMsgSend;
        rebindingObjcMsgSend.name = "objc_msgSend";
        rebindingObjcMsgSend.replacement = hook_msgSend;        // 替换成hook_msgSend函数
        rebindingObjcMsgSend.replaced = (void *)&orgin_objc_msgSend;    // 保存原始的objc_msgSend函数调用
        
        // hook objc_msgSendSuper2
        struct rebinding rebindingObjcMsgSendSuper2;
        rebindingObjcMsgSendSuper2.name = "objc_msgSendSuper2";
        rebindingObjcMsgSendSuper2.replacement = hook_msgSendSuper2;
        rebindingObjcMsgSendSuper2.replaced = (void *)&orgin_objc_msgSendSuper2;

        struct rebinding rebs[2] = {rebindingObjcMsgSend, rebindingObjcMsgSendSuper2};
        rebind_symbols(rebs, 2);
    });
}

void stopTrace(void)
{
    CallRecordEnable = NO;
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
    if (CallRecordEnable && pthread_main_np()) {
        // 一些记录操作
        printf("%s %s\n", object_getClassName(self), sel_getName(selector));
        
    }
    
    // 记录LR寄存器
    setLRRegisterValue(lr);
}

uintptr_t hook_objc_msgSend_after(BOOL is_objc_msgSendSuper)   // 为什么这里要判断super
{
    // 一些操作
    if (CallRecordEnable && pthread_main_np()) {
        
    }
    
    return getLRRegisterValue();
}
