//
//  SKProduct+LocalizedPrice.m
//  Taskdeck-iOS
//
//  Created by Karl Traunmüller on 20.08.13.
//  Copyright (c) 2013 microLARGE. All rights reserved.
//

#import "SKProduct+LocalizedPrice.h"

@implementation SKProduct (LocalizedPrice)

- (NSString *)localizedPrice
{
    NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
    [formatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [formatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [formatter setLocale:[self priceLocale]];
    
    NSString *str = [formatter stringFromNumber:[self price]];
    return str;
}

@end
