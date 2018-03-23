//
//  TourPageControllerDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "TourPageControllerDelegate.h"
#import "TourPageViewController.h"
#import "TourPage.h"

@implementation TourPageControllerDelegate

- (void)setup
{
}

- (void)resetPageController:(TourPageViewController *)pageController
{
    if (pageController.tourPage.imageName)
    {
        pageController.imageView.image = [UIImage imageNamed:pageController.tourPage.imageName];
    }
    
//    pageController.view.transform = self.initialTransform;
    CGAffineTransform t = self.initialTransform;
    pageController.imageCenterXConstraint.constant = t.tx;
    pageController.imageCenterYConstraint.constant = -t.ty;
    pageController.imageView.transform = CGAffineTransformMake(t.a, t.b, t.c, t.d, 0, 0);
    pageController.videoPlayerView.transform = pageController.imageView.transform;
//    pageController.view.layer.sublayerTransform = CATransform3DMakeAffineTransform(CGAffineTransformMake(t.a, t.b, t.c, t.d, 0, 0));
    [pageController.view layoutIfNeeded];
}

- (void)willPerformStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController
{    
}

- (void)performStep:(AnimationStep *)step inPageController:(TourPageViewController *)pageController
{
//    pageController.view.transform = step.transform;
    CGAffineTransform t = step.transform;
    pageController.imageCenterXConstraint.constant = t.tx;
    pageController.imageCenterYConstraint.constant = -t.ty;
    pageController.imageView.transform = CGAffineTransformMake(t.a, t.b, t.c, t.d, 0, 0);
    pageController.videoPlayerView.transform = pageController.imageView.transform;
//    pageController.view.layer.sublayerTransform = CATransform3DMakeAffineTransform(CGAffineTransformMake(t.a, t.b, t.c, t.d, 0, 0));
    [pageController.view layoutIfNeeded];
}

@end
