//
//  InAppPurchase.m
//  Computable
//
//  Created by Karl Traunmüller on 13.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "InAppPurchase.h"
#import "../../../Common/ThirdParty/CargoBay/CargoBay.h"
#import "../../../Common/Utilities/SKProduct+LocalizedPrice.h"

#import <StoreKit/StoreKit.h>

NSString *const InAppPurchaseWillStartNotification = @"InAppPurchaseWillStartNotification";
NSString *const InAppPurchaseDidFinishNotification = @"InAppPurchaseDidFinishNotification";

@implementation InAppPurchase
{
    NSString *_productId;
}

#pragma mark - InAppPurchae

- (id)initWithProductId:(NSString *)productId
{
    if (self = [super init])
    {
        _productId = productId;
        
        _isProductPurchased = [[NSUserDefaults standardUserDefaults] boolForKey:_productId];
#if TARGET_IPHONE_SIMULATOR
        _isProductPurchased = YES;
#endif
    }
    return self;
}

- (void)getProductInfo
{
    NSArray *productIds = @[ _productId ];
    [[CargoBay sharedManager] productsWithIdentifiers:[NSSet setWithArray:productIds]
                                              success:^(NSArray *products, NSArray *invalidIdentifiers) {
                                                  for (SKProduct *product in products)
                                                  {
                                                      if ([product.productIdentifier isEqualToString:_productId])
                                                      {
                                                          _localizedPrice = product.localizedPrice;
                                                          break;
                                                      }
                                                  }
                                              } failure:^(NSError *error) {
                                              }];
}

- (void)purchase
{
    if (![SKPaymentQueue canMakePayments])
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Error", nil)
                                                        message:NSLocalizedString(@"Payments not possible on this device", nil)
                                                       delegate:nil
                                              cancelButtonTitle:NSLocalizedString(@"Cancel", nil)
                                              otherButtonTitles:NSLocalizedString(@"Ok", nil), nil];
        [alert show];
        return;
    }
    
    [[NSNotificationCenter defaultCenter] postNotificationName:InAppPurchaseWillStartNotification object:nil];
    
    NSArray *productIds = @[ _productId ];
    [[CargoBay sharedManager] productsWithIdentifiers:[NSSet setWithArray:productIds]
                                              success:^(NSArray *products, NSArray *invalidIdentifiers) {
                                                  for (SKProduct *product in products)
                                                  {
                                                      if ([product.productIdentifier isEqualToString:_productId])
                                                      {
                                                          SKPayment *payment = [SKPayment paymentWithProduct:product];
                                                          [[SKPaymentQueue defaultQueue] addPayment:payment];
                                                          break;
                                                      }
                                                  }
                                              }
                                              failure:^(NSError *error) {
                                                  NSLog(@"error fetching products: %@", error);
                                                  [self purchaseDidFinishSuccessfully:NO];
                                              }];
}

- (void)purchaseDidFinishSuccessfully:(BOOL)success
{
    if (success)
    {
        NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
        [userDefaults setBool:YES forKey:_productId];
        [userDefaults synchronize];
        _isProductPurchased = YES;
    }
    [[NSNotificationCenter defaultCenter] postNotificationName:InAppPurchaseDidFinishNotification object:nil];
}

- (void)restore
{
    [[NSNotificationCenter defaultCenter] postNotificationName:InAppPurchaseWillStartNotification object:nil];
    
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
}

- (void)resetPurchase
{
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    [userDefaults removeObjectForKey:_productId];
    [userDefaults synchronize];
    _isProductPurchased = NO;
}

- (void)registerPaymentTransactionObserver
{
    [[CargoBay sharedManager] setPaymentQueueUpdatedTransactionsBlock:^(SKPaymentQueue *queue, NSArray *transactions) {
        for (SKPaymentTransaction *transaction in transactions)
        {
            switch (transaction.transactionState)
            {
                case SKPaymentTransactionStatePurchased:
                    NSLog(@"transaction purchased: %@", transaction.transactionIdentifier);
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                    [self purchaseDidFinishSuccessfully:YES];
                    break;
                    
                case SKPaymentTransactionStateFailed:
                    NSLog(@"transaction failed: %@", transaction.transactionIdentifier);
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                    [self purchaseDidFinishSuccessfully:NO];
                    break;
                    
                case SKPaymentTransactionStateRestored:
                    NSLog(@"transaction restored: %@", transaction.transactionIdentifier);
                    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
                    [self purchaseDidFinishSuccessfully:YES];
                    break;
                    
                default:
                    break;
            }
        }
    }];
    
    [[CargoBay sharedManager] setPaymentQueueRestoreCompletedTransactionsWithSuccess:^(SKPaymentQueue *queue) {
        [self purchaseDidFinishSuccessfully:YES];
    } failure:^(SKPaymentQueue *queue, NSError *error) {
        NSLog(@"transaction restore failed: %@", error);
        [self purchaseDidFinishSuccessfully:NO];
    }];
    
    [[SKPaymentQueue defaultQueue] addTransactionObserver:[CargoBay sharedManager]];
}

@end
