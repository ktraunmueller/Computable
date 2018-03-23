//
//  TourPage4ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage4ControllerDelegate.h"

@implementation TourPage4ControllerDelegate

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.91, 0.91),
                                                    iOS8_OR_LATER ?
                                                    CGAffineTransformMakeTranslation(-30, 490) :
                                                    CGAffineTransformMakeTranslation(-52, 302));
    
    AnimationStep *step1 = [AnimationStep new];
    step1.duration = 0.8;
    step1.options = UIViewAnimationOptionCurveEaseInOut;
    step1.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1, 1),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-68, 560) :
                                              CGAffineTransformMakeTranslation(-85, 320));
    
    AnimationStep *step2 = [AnimationStep new];
    step2.duration = 9;
    step2.options = UIViewAnimationOptionCurveEaseInOut;
    step2.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1, 1),
                                              iOS8_OR_LATER ?
                                              CGAffineTransformMakeTranslation(-68, 558) :
                                              CGAffineTransformMakeTranslation(-85, 319));
    
    AnimationStep *step3 = [AnimationStep new];
    step3.duration = 0.8;
    step3.options = UIViewAnimationOptionCurveEaseInOut;
    step3.transform = self.initialTransform;
    
    self.animationSteps = @[ step1, step2, step3 ];
}

@end
