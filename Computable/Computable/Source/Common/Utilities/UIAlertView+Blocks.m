//
//  UIAlertView+Blocks.m
//  sleep
//
//  Created by Karl Traunmüller on 09.05.14.
//  Copyright (c) 2014 Runtastic. All rights reserved.
//

#import "UIAlertView+Blocks.h"
#import <objc/runtime.h>

@interface RTAlertWrapper : NSObject

@property (nonatomic, copy) void(^completionBlock)(UIAlertView *alertView, NSInteger buttonIndex);

@end

@implementation RTAlertWrapper

#pragma mark - UIAlertViewDelegate

// Called when a button is clicked. The view will be automatically dismissed after this call returns
- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    if (self.completionBlock) {
        self.completionBlock(alertView, buttonIndex);
    }
}

//// Called when we cancel a view (eg. the user clicks the Home button). This is not called when the user clicks the cancel button.
//// If not defined in the delegate, we simulate a click in the cancel button
//- (void)alertViewCancel:(UIAlertView *)alertView
//{
//    // Just simulate a cancel button click
//    if (self.completionBlock) {
//        self.completionBlock(alertView, alertView.cancelButtonIndex);
//    }
//}

@end

static const char kRTAlertWrapper;

@implementation UIAlertView (Blocks)

#pragma mark - UIAlertView (Blocks)

- (void)showWithCompletion:(void(^)(UIAlertView *alertView, NSInteger buttonIndex))completion
{
    RTAlertWrapper *alertWrapper = [RTAlertWrapper new];
    alertWrapper.completionBlock = completion;
    self.delegate = alertWrapper;
    objc_setAssociatedObject(self, &kRTAlertWrapper, alertWrapper, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
    [self show];
}

@end
