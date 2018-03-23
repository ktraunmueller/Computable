//
//  TourPageControllerDelegate.h
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "AnimationStep.h"

@class TourPageViewController;

@interface TourPageControllerDelegate : NSObject

@property (nonatomic, assign) CGAffineTransform initialTransform;
@property (nonatomic, strong) NSArray *animationSteps;

- (void)setup;
- (void)resetPageController:(TourPageViewController *)pageController;
- (void)willPerformStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController;
- (void)performStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController;

@end
