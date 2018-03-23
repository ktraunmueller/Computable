//
//  AppDelegate.h
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

extern NSString *const ShowTourRequestedNotification;
extern NSString *const CloseTourRequestedNotification;

@class Settings;

@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (nonatomic, readonly) BOOL isFullVersionPurchased;
@property (nonatomic, readonly) NSString *fullVersionLocalizedPrice;

- (void)purchaseFullVersion;
- (void)restoreInAppPurchases;
- (void)runPostLaunchActions;

@end
