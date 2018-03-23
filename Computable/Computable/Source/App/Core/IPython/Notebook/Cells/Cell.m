//
//  Cell.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Cell.h"
#import "../Notebook.h"

@implementation Cell

#pragma mark - Cell

- (id)initWithNotebook:(Notebook *)notebook
{
    if (self = [super init])
    {
        _notebook = notebook;
    }
    return self;
}

- (void)invalidate
{
    _cachedHeight = 0;
}

@end
