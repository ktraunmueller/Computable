//
//  UIView+Wiggle.m
//  Computable
//
//  Created by Karl Traunmüller on 18.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIView+Wiggle.h"

#import <objc/runtime.h>

@implementation UIView (Wiggle)

static const CGFloat kWiggleBounceY = 2.0;
static const CGFloat kWiggleBounceDuration = 0.18;
static const CGFloat kWiggleBounceDurationVariance = 0.025;

static const CGFloat kWiggleRotateAngle = 0.018;
static const CGFloat kWiggleRotateDuration = 0.15;
static const CGFloat kWiggleRotateDurationVariance = 0.025;

static NSString *const kWiggleAnimationKey = @"wiggle";
static NSString *const kRotationAnimationKey = @"rotation";
static NSString *const kBounceAnimationKey = @"bounce";

static char RotationKey;
static char BounceKey;

- (CAAnimation *)rotationAnimation
{
	return (CAAnimation *)objc_getAssociatedObject(self, &RotationKey);
}

- (void)setRotationAnimation:(CAAnimation *)animation
{
	objc_setAssociatedObject(self, &RotationKey, animation, OBJC_ASSOCIATION_RETAIN);
}

- (CAAnimation *)bounceAnimation
{
	return (CAAnimation *)objc_getAssociatedObject(self, &BounceKey);
}

- (void)setBounceAnimation:(CAAnimation *)animation
{
	objc_setAssociatedObject(self, &BounceKey, animation, OBJC_ASSOCIATION_RETAIN);
}

- (void)startWiggleAnimation
{
    CAAnimation *rotationAnimation = [self makeRotationAnimation];
    [self setRotationAnimation:rotationAnimation];
    [self.layer addAnimation:rotationAnimation forKey:@"rotation"];
    
    CAAnimation *bounceAnimation = [self makeBounceAnimation];
    [self setBounceAnimation:bounceAnimation];
    [self.layer addAnimation:bounceAnimation forKey:@"bounce"];
    
    self.transform = CGAffineTransformIdentity;
}

- (void)stopWiggleAnimation
{
    [self.layer removeAllAnimations];
    [self setRotationAnimation:nil];
    [self setBounceAnimation:nil];
}

- (BOOL)isWiggleAnimationRunning
{
    return [self rotationAnimation] != nil;
}

- (CAAnimation *)makeRotationAnimation
{
    CAKeyframeAnimation *animation = [CAKeyframeAnimation animationWithKeyPath:@"transform.rotation.z"];
    animation.values = @[ @(-kWiggleRotateAngle), @(kWiggleRotateAngle) ];
    
    animation.autoreverses = YES;
    animation.duration = [self randomizeInterval:kWiggleRotateDuration
                                    withVariance:kWiggleRotateDurationVariance];
    animation.repeatCount = HUGE_VALF;
    
    return animation;
}

- (CAAnimation *)makeBounceAnimation
{
    CAKeyframeAnimation* animation = [CAKeyframeAnimation animationWithKeyPath:@"transform.translation.y"];
    animation.values = @[ @(kWiggleBounceY), @(0.0) ];
    
    animation.autoreverses = YES;
    animation.duration = [self randomizeInterval:kWiggleBounceDuration
                                    withVariance:kWiggleBounceDurationVariance];
    animation.repeatCount = HUGE_VALF;
    
    return animation;
}

- (NSTimeInterval)randomizeInterval:(NSTimeInterval)interval withVariance:(double)variance
{
    double random = (arc4random_uniform(1000) - 500.0) / 500.0;
    return interval + variance * random;
}

@end
