//
//  IPythonApplicationTests.m
//  Computable
//
//  Created by Karl Traunmüller on 31.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>

#import "../../Source/App/Core/IPython/IPythonApplication.h"

@interface IPythonApplicationTests : SenTestCase
@end

@implementation IPythonApplicationTests

- (void)testStartStop
{
    [IPythonApplication start:^{
        STAssertTrue([IPythonApplication isRunning], @"expected state running");
        [IPythonApplication stop:^{
            STAssertFalse([IPythonApplication isRunning], @"expected state stopped");
        }];
    }];
}

@end
