//
//  SKProduct+LocalizedPrice.h
//  Taskdeck-iOS
//
//  Created by Karl Traunmüller on 20.08.13.
//  Copyright (c) 2013 microLARGE. All rights reserved.
//

#import <StoreKit/StoreKit.h>

@interface SKProduct (LocalizedPrice)

- (NSString *)localizedPrice;

@end
