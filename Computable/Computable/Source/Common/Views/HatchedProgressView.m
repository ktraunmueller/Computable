//
//  HatchedProgressView.m
//  Computable
//
//  Created by Karl Traunmüller on 18.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "HatchedProgressView.h"

#import <QuartzCore/QuartzCore.h>

static const CGFloat kHatchWidth = 8;
static const CGFloat kHatchSpacing = 20;
static const CFTimeInterval kAnimationDuration = 0.7;

@implementation HatchedProgressView
{
    NSMutableArray *_hatchLayers;
}

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame])
    {
        [self setupHatches];
    }
    return self;
}

- (void)setupHatches
{
    [_hatchLayers makeObjectsPerformSelector:@selector(removeFromSuperlayer)];

    _hatchLayers = [NSMutableArray array];
    NSInteger numberOfHatches = CGRectGetWidth(self.frame) / kHatchWidth;
    CGFloat height = 2 * CGRectGetHeight(self.frame);
    for (NSInteger i = 0; i < numberOfHatches; i++)
    {
        CALayer *layer = [CALayer layer];
        layer.frame = CGRectMake(i * kHatchSpacing - kHatchWidth, -height / 4, kHatchWidth, height);
        layer.transform = CATransform3DMakeRotation(M_PI_4, 0, 0, 1);
        layer.backgroundColor = [UIColor colorWithWhite:0.98 alpha:1].CGColor;
        [_hatchLayers addObject:layer];
        [self.layer addSublayer:layer];
    }
}

- (void)startAnimating
{
    [CATransaction begin];
    [CATransaction setCompletionBlock:^{
        
    }];
    [CATransaction setAnimationDuration:kAnimationDuration];
    for (CALayer *layer in _hatchLayers)
    {
        CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"transform.translation.x"];
        animation.duration = kAnimationDuration;
        animation.repeatCount = HUGE_VALF;
        animation.removedOnCompletion = NO;
        animation.autoreverses = NO;
        animation.byValue = [NSNumber numberWithFloat:kHatchSpacing];
        [layer addAnimation:animation forKey:@"move"];

    }
    [CATransaction commit];
}

- (void)stopAnimating
{
    for (CALayer *layer in _hatchLayers)
    {
        [layer removeAllAnimations];
    }
}

@end
