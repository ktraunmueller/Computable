//
//  Analytics.h
//  Computable
//
//  Created by Karl Traunmüller on 14.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Analytics : NSObject

+ (void)startTracking;
+ (void)trackScreenView:(NSString *)screen;
+ (void)trackEventWithCategory:(NSString *)category action:(NSString *)action label:(NSString *)label value:(NSNumber *)value;

@end
