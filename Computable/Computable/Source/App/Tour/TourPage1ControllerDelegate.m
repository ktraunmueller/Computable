//
//  TourPage1ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage1ControllerDelegate.h"

@implementation TourPage1ControllerDelegate

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.5, 0.5),
                                                    CGAffineTransformMakeTranslation(0, 10)); // less y => up
    
    AnimationStep *step1 = [AnimationStep new];
    step1.duration = 1;
    step1.options = UIViewAnimationOptionCurveEaseInOut;
    step1.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1, 1),
                                              CGAffineTransformMakeTranslation(0, 180));
    
    AnimationStep *step2 = [AnimationStep new];
    step2.duration = 3;
    step2.delay = 0.2;
    step2.options = UIViewAnimationOptionCurveEaseInOut;
    step2.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1, 1),
                                              CGAffineTransformMakeTranslation(0, -100));
    
    AnimationStep *step3 = [AnimationStep new];
    step3.duration = 1;
//    step3.delay = 0.2;
    step3.options = UIViewAnimationOptionCurveEaseInOut;
    step3.transform = self.initialTransform;
    
    self.animationSteps = @[ step1, step2, step3 ];
}

@end
