//
//  AppDelegate.m
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "AppDelegate.h"
#import "Colors.h"
#import "Core/Analytics/Analytics.h"
#import "Core/Dropbox/Dropbox.h"
#import "Core/Dropbox/DropboxSync.h"
#import "Core/InAppPurchase/InAppPurchase.h"
#import "../Common/Utilities/UIAlertView+Blocks.h"

#import <HockeySDK/HockeySDK.h>

NSString *const ShowTourRequestedNotification = @"ShowTourRequestedNotification";
NSString *const CloseTourRequestedNotification = @"CloseTourRequestedNotification";

static NSString *const kHockeyAppId = @"--REMOVED--";
static NSString *const kFullVersionProductId = @"ComputableFullVersion";
static NSString *const kResetPurchaseMarker = @"ComputableFullVersionReset";

@implementation AppDelegate
{
    InAppPurchase *_fullVersionPurchase;
    BOOL _resetPurchaseRequired;
}

#pragma mark - Properties

- (BOOL)isFullVersionPurchased
{
    return _fullVersionPurchase.isProductPurchased;
}

- (NSString *)fullVersionLocalizedPrice
{
    return _fullVersionPurchase.localizedPrice;
}

#pragma mark - AppDelegate

- (void)purchaseFullVersion
{
    [_fullVersionPurchase purchase];
}

- (void)restoreInAppPurchases
{
    [_fullVersionPurchase restore];
}

- (void)runPostLaunchActions
{
    if (_resetPurchaseRequired)
    {
        _resetPurchaseRequired = NO;
        NSString *title = NSLocalizedString(@"Restore Purchase", nil);
        NSString *message = NSLocalizedString(@"If you have previously purchased Computable, please click the Restore button to restore your purchase. Sorry for the inconvenience.", nil);
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"Cancel", nil)
                                              otherButtonTitles:NSLocalizedString(@"Restore", nil), nil];
        [alert showWithCompletion:^(UIAlertView *alertView, NSInteger buttonIndex) {
            switch (buttonIndex)
            {
                case 0: // Cancel
                    break;
                case 1:
                    [self restoreInAppPurchases];
                    break;
                default:
                    break;
            }
        }];
    }
}

#pragma mark - UIApplicationDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //    for (NSString *family in [UIFont familyNames])
    //    {
    //        NSLog(@"%@: %@", family, [UIFont fontNamesForFamilyName:family]);
    //    }
    
#if TARGET_IPHONE_SIMULATOR
    NSLog(@"Documents Directory: %@", [[[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask] lastObject]);
#endif
    
    [[BITHockeyManager sharedHockeyManager] configureWithIdentifier:kHockeyAppId];
    [[[BITHockeyManager sharedHockeyManager] crashManager] setCrashManagerStatus:BITCrashManagerStatusAutoSend];
    [[BITHockeyManager sharedHockeyManager] startManager];
    
    [Analytics startTracking];
    [Analytics trackEventWithCategory:@"App" action:@"launch" label:nil value:nil];
    
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    _fullVersionPurchase = [[InAppPurchase alloc] initWithProductId:kFullVersionProductId];
    _resetPurchaseRequired = _fullVersionPurchase.isProductPurchased && [userDefaults objectForKey:kResetPurchaseMarker] == nil;
    if (_resetPurchaseRequired)
    {
        [_fullVersionPurchase resetPurchase];
        [userDefaults setBool:YES forKey:kResetPurchaseMarker];
        [userDefaults synchronize];
    }
    [_fullVersionPurchase registerPaymentTransactionObserver];
    if (!_fullVersionPurchase.isProductPurchased)
    {
        [_fullVersionPurchase getProductInfo];
    }
    
    [Colors initialize];
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    
    [Dropbox setup];
    if ([Dropbox isLinked])
    {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [DropboxSync start];
        });
    }
    
    return YES;
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    [DropboxSync stop];
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    if ([url.absoluteString hasPrefix:kDropboxUrlScheme])
    {
        return [Dropbox openURL:url];
    }
	return NO;
}

@end
