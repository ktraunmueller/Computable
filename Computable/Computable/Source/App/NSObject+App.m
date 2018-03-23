//
//  NSObject+App.m
//  Computable
//
//  Created by Karl Traunmüller on 26.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSObject+App.h"

@implementation NSObject (App)

- (AppDelegate *)appDelegate
{
    return (AppDelegate *)[UIApplication sharedApplication].delegate;
}

@end