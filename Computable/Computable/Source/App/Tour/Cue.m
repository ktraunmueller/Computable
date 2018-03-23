//
//  Cue.m
//  Computable
//
//  Created by Karl Traunmüller on 11.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Cue.h"

Cue *kNopCue;

@implementation Cue

+ (void)initialize
{
    kNopCue = [Cue cueWithTop:NAN hOffset:NAN scale:NAN alpha:NAN];
}

+ (Cue *)cueWithTop:(CGFloat)top hOffset:(CGFloat)hOffset scale:(CGFloat)scale alpha:(CGFloat)alpha
{
    Cue *cue = [Cue new];
    cue.top = top;
    cue.hOffset = hOffset;
    cue.scale = scale;
    cue.alpha = alpha;
    return cue;
}

- (id)init
{
    if (self = [super init])
    {
        _top = NAN;
        _hOffset = NAN;
        _scale = NAN;
        _alpha = NAN;
    }
    return self;
}

@end
