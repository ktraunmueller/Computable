//
//  UIAlertView+Blocks.h
//  sleep
//
//  Created by Karl Traunmüller on 09.05.14.
//  Copyright (c) 2014 Runtastic. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIAlertView (Blocks)

- (void)showWithCompletion:(void(^)(UIAlertView *alertView, NSInteger buttonIndex))completion;

@end
