//
//  Completion.m
//  Computable
//
//  Created by Karl Traunmüller on 05.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Completion.h"

@implementation Completion

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ (%@)", _name, _type];
}

@end
