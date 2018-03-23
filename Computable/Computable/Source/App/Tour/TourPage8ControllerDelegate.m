//
//  TourPage8ControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPage8ControllerDelegate.h"
#import "TourPageViewController.h"
#import "TourPage.h"
#import "AnimationStep.h"
#import "../AppDelegate.h"

@implementation TourPage8ControllerDelegate
{
    UIButton *_button;
    CGPoint _startCenter, _endCenter;
}

- (void)closeTour:(id)sender
{
    [[NSNotificationCenter defaultCenter] postNotificationName:CloseTourRequestedNotification object:self];
}

- (void)setup
{
    self.initialTransform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.8, 0.8),
                                                    CGAffineTransformMakeTranslation(0, 120)); // less y => up
    
    AnimationStep *step1 = [AnimationStep new];
    step1.duration = 1.5;
    step1.options = UIViewAnimationOptionCurveEaseInOut;
    step1.transform = CGAffineTransformConcat(CGAffineTransformMakeScale(0.8, 0.8),
                                              CGAffineTransformMakeTranslation(0, 600));
    
    self.animationSteps = @[ step1];
    
    _button = [[UIButton alloc] initWithFrame:CGRectMake(0, 0, 200, 40)];
    [_button addTarget:self action:@selector(closeTour:) forControlEvents:UIControlEventTouchUpInside];
    _button.layer.cornerRadius = 4;
    _button.backgroundColor = [UIColor colorWithRed:132/255.0 green:204/255.0 blue:28/255.0 alpha:1.0];
    _button.titleLabel.font = [UIFont boldSystemFontOfSize:20];
    [_button setTitleEdgeInsets:UIEdgeInsetsMake(-10, -20, -10, -20)];
    [_button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_button setTitle:NSLocalizedString(@"Get Started", nil) forState:UIControlStateNormal];
}

- (void)resetPageController:(TourPageViewController *)pageController
{
    [super resetPageController:pageController];
    
    [_button removeFromSuperview];
}

- (void)willPerformStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController
{
    [super willPerformStep:step inPageController:pageController];
    
    CGFloat height = CGRectGetHeight(pageController.tourPage.pageContainerView.bounds);
    _startCenter = CGPointMake(CGRectGetMidX(pageController.tourPage.pageContainerView.bounds), -height);
    _endCenter = _startCenter;
    _endCenter.y = height / 2;
    
    _button.center = _startCenter;
    [pageController.tourPage.pageContainerView addSubview:_button];
}

- (void)performStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController
{
    [super performStep:step inPageController:pageController];
    
    _button.center = _endCenter;
}

@end
