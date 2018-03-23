//
//  Analytics.m
//  Computable
//
//  Created by Karl Traunmüller on 14.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Analytics.h"
#import <GAI.h>
#import <GAIFields.h>
#import <GAIDictionaryBuilder.h>

static NSString *const kTrackingId = @"--REMOVED--";

@implementation Analytics

+ (void)startTracking
{
    [[GAI sharedInstance] trackerWithTrackingId:kTrackingId];
    [[[GAI sharedInstance] defaultTracker] set:kGAIAppName value:@"Computable"];
}

+ (void)trackScreenView:(NSString *)screen
{
    [[[GAI sharedInstance] defaultTracker] set:kGAIScreenName value:screen];
    [[[GAI sharedInstance] defaultTracker] send:[[GAIDictionaryBuilder createAppView] build]];
    [[[GAI sharedInstance] defaultTracker] set:kGAIScreenName value:nil];
}

+ (void)trackEventWithCategory:(NSString *)category action:(NSString *)action label:(NSString *)label value:(NSNumber *)value
{
    NSMutableDictionary *parameters = [[GAIDictionaryBuilder createEventWithCategory:category action:action label:label value:value] build];
    if (parameters == nil)
    {
        return;
    }
    [[[GAI sharedInstance] defaultTracker] send:parameters];
}

@end
