//
//  Tests.m
//  Tests
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "../../../Source/App/Core/Python/Parser/PythonParser.h"
#import <SenTestingKit/SenTestingKit.h>

@interface ParserTests : SenTestCase

@end

@implementation ParserTests
{
    PythonParser *parser;
}

- (void)setUp
{
    [super setUp];
    
    parser = [[PythonParser alloc] init];
}

//- (void)testParser
//{
//    NSString *code =
//    @"import sys\n"
//    @"import platform'n"
//    @"print 'This is Python', platform.python_version()";
//
//    [parser parse:code];
//}

@end
