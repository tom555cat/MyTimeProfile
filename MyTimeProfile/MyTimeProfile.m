//
//  MyTimeProfile.m
//  MyTimeProfile
//
//  Created by tongleiming on 2021/6/28.
//

#import "MyTimeProfile.h"
#include "TPCallTrace.h"


@implementation MyTimeProfile

+ (void)load
{
    dispatch_async(dispatch_get_main_queue(), ^{
        hookObjcMsgSend();
    });
}





@end
