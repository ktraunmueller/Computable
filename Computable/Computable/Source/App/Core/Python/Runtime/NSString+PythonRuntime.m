//
//  NSString+PythonRuntime.m
//  Computable
//
//  Created by Karl Traunmüller on 18.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSString+PythonRuntime.h"
#import "PythonRuntime.h"

@implementation NSString (PythonRuntime)

- (NSString *)stringByStrippingPythonPath
{
    NSString *pythonPath = [PythonRuntime runtimePath];
    NSString *result = self;
    NSRange pathRange = [self rangeOfString:pythonPath];
    if (pathRange.location != NSNotFound)
    {
        result = [result stringByReplacingCharactersInRange:pathRange withString:@""];
    }
    return result;
}

@end
