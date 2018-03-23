//
//  MarkdownToken.m
//  Computable
//
//  Created by Karl Traunmüller on 01.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "MarkdownToken.h"

@implementation MarkdownToken

#pragma mark - Statics

+ (MarkdownToken *)tokenWithKind:(MarkdownTokenKind)kind text:(NSString *)text
{
    return [[MarkdownToken alloc] initWithKind:kind text:text];
}

#pragma mark - PythonToken

- (id)initWithKind:(MarkdownTokenKind)kind text:(NSString *)text
{
    if (self = [super init])
    {
        _kind = kind;
        _text = [text copy];
    }
    return self;
}

- (NSString *)description
{
    return _text;
}

@end
