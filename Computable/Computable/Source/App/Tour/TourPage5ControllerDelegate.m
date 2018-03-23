//
//  TourPage5ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage5ControllerDelegate.h"

@implementation TourPage5ControllerDelegate

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(iOS8_OR_LATER ?
                                                    CGAffineTransformMakeScale(0.91, 0.91) :
                                                    CGAffineTransformMakeScale(0.20, 0.20),
                                                    iOS8_OR_LATER ?
                                                    CGAffineTransformMakeTranslation(-30, 335) :
                                                    CGAffineTransformMakeTranslation(-290, 455));

    AnimationStep *step1 = [AnimationStep new];
    step1.duration = 0.8;
    step1.options = UIViewAnimationOptionCurveEaseInOut;
    step1.transform = CGAffineTransformConcat(iOS8_OR_LATER ?
                                              CGAffineTransformMakeScale(0.73, 0.73) :
                                              CGAffineTransformMakeScale(0.14, 0.14),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-20, 238) :
                                              CGAffineTransformMakeTranslation(-280, 410));
    
    AnimationStep *step2 = [AnimationStep new];
    step2.duration = 16;
    step2.options = UIViewAnimationOptionCurveEaseInOut;
    step2.transform = CGAffineTransformConcat(iOS8_OR_LATER ?
                                              CGAffineTransformMakeScale(0.73, 0.73):
                                              CGAffineTransformMakeScale(0.14, 0.14),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-20, 235) :
                                              CGAffineTransformMakeTranslation(-280, 405));

    AnimationStep *step3 = [AnimationStep new];
    step3.duration = 0.8;
    step3.delay = 0.3;
    step3.options = UIViewAnimationOptionCurveEaseInOut;
    step3.transform = self.initialTransform;
    
    self.animationSteps = @[ step1, step2, step3 ];
}

@end
