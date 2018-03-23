//
//  UIViewController+Containment.m
//  Computable
//
//  Created by Karl Traunmüller on 11.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIViewController+Containment.h"

@implementation UIViewController (Containment)

- (void)installChildViewController:(UIViewController *)childController inView:(UIView *)parentView
{
    [self installChildViewController:childController inView:parentView edgeInsets:UIEdgeInsetsZero];
}

- (void)installChildViewController:(UIViewController *)childController inView:(UIView *)parentView edgeInsets:(UIEdgeInsets)edgeInsets
{
    if (childController == nil || parentView == nil)
    {
        return;
    }
    [self addChildViewController:childController];
    childController.view.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    childController.view.frame = UIEdgeInsetsInsetRect(parentView.bounds, edgeInsets);
    [parentView addSubview:childController.view];
    [childController didMoveToParentViewController:self];
}

- (void)uninstallChildViewController:(UIViewController *)childController
{
    [childController willMoveToParentViewController:nil];
    [childController.view removeFromSuperview];
    [childController removeFromParentViewController];
}

@end
