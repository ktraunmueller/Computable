//
//  UIViewController+Containment.h
//  Computable
//
//  Created by Karl Traunmüller on 11.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIViewController (Containment)

- (void)installChildViewController:(UIViewController *)childController inView:(UIView *)parentView;
- (void)installChildViewController:(UIViewController *)childController inView:(UIView *)parentView edgeInsets:(UIEdgeInsets)edgeInsets;

- (void)uninstallChildViewController:(UIViewController *)childController;

@end
