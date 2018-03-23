//
//  TourPage7ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage7ControllerDelegate.h"

@implementation TourPage7ControllerDelegate

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.91, 0.91),
                                                    iOS8_OR_LATER ?
                                                    CGAffineTransformMakeTranslation(-30, 335) :
                                                    CGAffineTransformMakeTranslation(-40, 212));
    
    AnimationStep *step1 = [AnimationStep new];
    step1.duration = 1;
    step1.options = UIViewAnimationOptionCurveEaseInOut;
    step1.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1, 1),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-10, 388) :
                                              CGAffineTransformMakeTranslation(20, 220));
    
    AnimationStep *step2 = [AnimationStep new];
    step2.duration = 1;
    step2.delay = 2.1;
    step2.options = UIViewAnimationOptionCurveEaseInOut;
    step2.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.91, 0.91),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-30, 335) :
                                              CGAffineTransformMakeTranslation(-50, 210));
    
    AnimationStep *step3 = [AnimationStep new];
    step3.duration = 5.0;
    step3.options = UIViewAnimationOptionCurveEaseInOut;
    step3.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.91, 0.91),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-30, 340) :
                                              CGAffineTransformMakeTranslation(-50, 215));
    
    AnimationStep *step4 = [AnimationStep new];
    step4.duration = 0.8;
    step4.delay = 0.2;
    step4.options = UIViewAnimationOptionCurveEaseInOut;
    step4.transform = self.initialTransform;
    
    self.animationSteps = @[ step1, step2, step3, step4 ];
}

@end
