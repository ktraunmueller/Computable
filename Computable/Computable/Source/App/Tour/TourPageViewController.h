//
//  TourPageViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const EndTourNotification;

@class TourPage;
@class TourPageControllerDelegate;

@interface TourPageViewController : UIViewController

@property (nonatomic, weak) IBOutlet UIImageView *imageView;
@property (nonatomic, weak) IBOutlet NSLayoutConstraint *imageCenterXConstraint;
@property (nonatomic, weak) IBOutlet NSLayoutConstraint *imageCenterYConstraint;
@property (nonatomic, readonly) UIView *videoPlayerView;

@property (nonatomic, weak) TourPage *tourPage;
@property (nonatomic, strong) TourPageControllerDelegate *delegate;

- (void)reset;
- (void)start;

@end
