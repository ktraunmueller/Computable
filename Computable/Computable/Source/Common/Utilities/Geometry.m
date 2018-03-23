//
//  Geometry.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Geometry.h"

@implementation Geometry

+ (CGPoint)add:(CGPoint)vector to:(CGPoint)point
{
    return CGPointMake(point.x + vector.x, point.y + vector.y);
}

+ (CGPoint)subtract:(CGPoint)vector from:(CGPoint)point
{
    return CGPointMake(point.x - vector.x, point.y - vector.y);
}

+ (CGPoint)rotatePoint:(CGPoint)p by:(CGFloat)theta
{
    return CGPointMake(p.x * cosf(theta) - p.y * sinf(theta),
                       p.x * sinf(theta) + p.y * cosf(theta));
}

+ (CGPoint)randomlyOffset:(CGPoint)point by:(CGFloat)maxDistance
{
    return CGPointMake(point.x + arc4random_uniform(maxDistance) - maxDistance / 2,
                       point.y + arc4random_uniform(maxDistance) - maxDistance / 2);
}

@end
