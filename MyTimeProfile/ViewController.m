//
//  ViewController.m
//  MyTimeProfile
//
//  Created by tongleiming on 2021/6/28.
//

#import "ViewController.h"
#import "TPCallTrace.h"
#import "Child.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    startTrace("测试");
    [self myTest];
    
    Child *c = [Child new];
    [c testChild];
    stopTrace();
}

- (void)myTest
{
    [self myTest1];
}

- (void)myTest1
{
    
}

@end
