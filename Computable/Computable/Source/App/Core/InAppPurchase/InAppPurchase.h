//
//  InAppPurchase.h
//  Computable
//
//  Created by Karl Traunmüller on 13.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString *const InAppPurchaseWillStartNotification;
extern NSString *const InAppPurchaseDidFinishNotification;

@interface InAppPurchase : NSObject

@property (nonatomic, readonly) BOOL isProductPurchased;
@property (nonatomic, readonly) NSString *localizedPrice;

- (id)initWithProductId:(NSString *)productId;

- (void)registerPaymentTransactionObserver;
- (void)getProductInfo;
- (void)purchase;
- (void)restore;
- (void)resetPurchase;

@end
