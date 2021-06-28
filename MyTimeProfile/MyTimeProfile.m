//
//  MyTimeProfile.m
//  MyTimeProfile
//
//  Created by tongleiming on 2021/6/28.
//

#import "MyTimeProfile.h"
#include "fishhook.h"

// 局部变量，使用的函数指针
void (*orgin_objc_msgSend)(void);


#warning 无限参数的函数怎么表示?
#warning objc_msgSend不是无限参数吗
extern void hook_msgSend();

@implementation MyTimeProfile

+ (void)load
{
    dispatch_async(dispatch_get_main_queue(), ^{
        hookObjcMsgSend();
    });
}

void hookObjcMsgSend()
{
    struct rebinding rebindingObjcMsgSend;
    rebindingObjcMsgSend.name = "objc_msgSend";
    rebindingObjcMsgSend.replacement = hook_msgSend;        // 替换成hook_msgSend函数
    rebindingObjcMsgSend.replaced = (void *)&orgin_objc_msgSend;    // 保存原始的objc_msgSend函数调用
    
    struct rebinding rebs[1] = {rebindingObjcMsgSend};
    rebind_symbols(rebs, 1);
}

@end
