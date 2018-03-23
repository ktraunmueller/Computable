//
//  TourPage3ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage3ControllerDelegate.h"
#import "TourPageViewController.h"
#import "TourPage.h"
#import "../Core/Settings/Settings.h"
#import "../../Common/Utilities/Geometry.h"

@implementation TourPage3ControllerDelegate

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.6, 0.6),
                                                    CGAffineTransformMakeTranslation(0, 30)); // less y => up
    
    AnimationStep *step1 = [AnimationStep new];
    step1.duration = 1.5;
    step1.options = UIViewAnimationOptionCurveEaseInOut;
    step1.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1.15, 1.15),
                                              CGAffineTransformMakeTranslation(0, 170));
    
    AnimationStep *step2 = [AnimationStep new];
    step2.duration = 3;
    step2.delay = 2;
    step2.options = UIViewAnimationOptionCurveLinear;
    step2.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1.15, 1.15),
                                              CGAffineTransformMakeTranslation(0, 160));
    step2.image = [UIImage imageNamed:@"tour3_1"];
    
    AnimationStep *step3 = [AnimationStep new];
    step3.duration = 3;
    step3.delay = 0;
    step3.options = UIViewAnimationOptionCurveLinear;
    step3.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1.15, 1.15),
                                              CGAffineTransformMakeTranslation(0, 150));
    step3.image = [UIImage imageNamed:@"tour3_2"];
    
    AnimationStep *step4 = [AnimationStep new];
    step4.duration = 3;
    step4.delay = 0;
    step4.options = UIViewAnimationOptionCurveLinear;
    step4.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(1.15, 1.15),
                                              CGAffineTransformMakeTranslation(0, 140));
    step4.image = [UIImage imageNamed:@"tour3_3"];
    
    AnimationStep *step5 = [AnimationStep new];
    step5.duration = 1.5;
    step5.delay = 0;
    step5.options = UIViewAnimationOptionCurveEaseInOut;
    step5.transform = self.initialTransform;
    step5.image = [UIImage imageNamed:@"tour3"];
    
    self.animationSteps = @[ step1, step2, step3, step4, step5 ];
}

- (void)willPerformStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController
{
    [super willPerformStep:step inPageController:pageController];

    if (step.image == nil)
    {
        return;
    }
    [UIView animateWithDuration:0.2
                          delay:step.delay
                        options:UIViewAnimationOptionCurveEaseInOut
                     animations:^{
                         pageController.imageView.alpha = 0.3;
                     } completion:^(BOOL finished) {
                         pageController.imageView.image = step.image;
                         [UIView animateWithDuration:0.2
                                               delay:0
                                             options:UIViewAnimationOptionCurveEaseInOut
                                          animations:^{
                                              pageController.imageView.alpha = 1;
                                          } completion:nil];
                     }];
}

@end
