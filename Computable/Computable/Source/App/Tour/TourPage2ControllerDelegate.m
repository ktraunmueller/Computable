//
//  TourPage2ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage2ControllerDelegate.h"
#import "TourPageViewController.h"
#import "TourPage.h"
#import "../Core/Settings/Settings.h"
#import "../../Common/Utilities/Geometry.h"

@implementation TourPage2ControllerDelegate
{
    AnimationStep *_step1;
    NSMutableArray *_labels;
}

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.6, 0.6),
                                                    CGAffineTransformMakeTranslation(0, 30)); // less y => up
    
    _step1 = [AnimationStep new];
    _step1.duration = 1.5;
    _step1.options = UIViewAnimationOptionCurveEaseInOut;
    _step1.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.55, 0.55),
                                               CGAffineTransformMakeTranslation(0, 30));
    
    AnimationStep *step2 = [AnimationStep new];
    step2.duration = 1.5;
    step2.delay = 1;
    step2.options = UIViewAnimationOptionCurveEaseInOut;
    step2.transform = self.initialTransform;
    
    self.animationSteps = @[ _step1, step2 ];
    
    FontSetting *font = [Settings instance].modernTextFont;
    
    NSArray *texts = @[ @"IPython", @"NumPy", @"SciPy", @"Matplotlib", @"SymPy", @"Pandas" ];
    _labels = [NSMutableArray array];
    for (NSString *text in texts)
    {
        UILabel *label = [[UILabel alloc] initWithFrame:CGRectZero];
        [_labels addObject:label];
        label.font = [font.boldFont fontWithSize:30];
        label.text = text;
        [label sizeToFit];
    }
}

- (void)resetPageController:(TourPageViewController *)pageController
{
    [super resetPageController:pageController];
    
    [_labels makeObjectsPerformSelector:@selector(removeFromSuperview)];
}

- (void)willPerformStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController
{
    [super willPerformStep:step inPageController:pageController];

    if (step == _step1)
    {
        CGPoint center = CGPointMake(CGRectGetMidX(pageController.tourPage.pageContainerView.bounds),
                                     CGRectGetMidY(pageController.tourPage.pageContainerView.bounds));
        
        NSTimeInterval delay = 0;
        CGFloat angle = 0;
        CGFloat dAngle = 2 * M_PI / [_labels count];
        for (UILabel *label in _labels)
        {
            CGFloat dx = 160;
            CGPoint direction = CGPointMake(dx, 0);
            direction = [Geometry rotatePoint:direction by:angle];
            angle += dAngle;
            CGFloat scale = 2;
            CGPoint offset = CGPointMake(direction.x * 0.3, direction.y * 0.3);
            
            label.alpha = 1;
            label.center = [Geometry add:offset to:center];
            label.transform = CGAffineTransformIdentity;
            [pageController.tourPage.pageContainerView addSubview:label];
            
            [UIView animateWithDuration:3.5
                                  delay:delay
                                options:UIViewAnimationOptionCurveLinear
                             animations:^{
                                 label.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(scale, scale),
                                                                           CGAffineTransformMakeTranslation(direction.x, direction.y));
                                 label.alpha = 0;
                             } completion:nil];
        }
    }
}

@end
