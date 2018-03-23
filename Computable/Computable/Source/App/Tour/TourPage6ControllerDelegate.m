//
//  TourPage6ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage6ControllerDelegate.h"

@implementation TourPage6ControllerDelegate

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.91, 0.91),
                                                    iOS8_OR_LATER ?
                                                    CGAffineTransformMakeTranslation(-30, 535) :
                                                    CGAffineTransformMakeTranslation(-52, 328));

    AnimationStep *step1 = [AnimationStep new];
    step1.duration = 0.8;
    step1.options = UIViewAnimationOptionCurveEaseInOut;
    step1.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1, 1),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-68, 605) :
                                              CGAffineTransformMakeTranslation(-85, 345));
    
    AnimationStep *step2 = [AnimationStep new];
    step2.duration = 26;
    step2.options = UIViewAnimationOptionCurveEaseInOut;
    step2.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1, 1),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-68, 600) :
                                              CGAffineTransformMakeTranslation(-85, 348));
    
    AnimationStep *step3 = [AnimationStep new];
    step3.duration = 0.8;
    step3.delay = 0.5;
    step3.options = UIViewAnimationOptionCurveEaseInOut;
    step3.transform = self.initialTransform;
    
    self.animationSteps = @[ step1, step2, step3 ];
}

@end
