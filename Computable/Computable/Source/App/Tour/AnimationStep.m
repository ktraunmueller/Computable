//
//  AnimationStep.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "AnimationStep.h"

@implementation AnimationStep

+ (NSTimeInterval)totalDuration:(NSArray *)animationSteps
{
    NSTimeInterval duration = 0;
    for (AnimationStep *step in animationSteps)
    {
        duration += step.delay + step.duration;
    }
    return duration;
}

@end
