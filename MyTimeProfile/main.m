//
//  main.m
//  MyTimeProfile
//
//  Created by tongleiming on 2021/6/28.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

struct {
    
} StackTest;

int main(int argc, char * argv[]) {
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
