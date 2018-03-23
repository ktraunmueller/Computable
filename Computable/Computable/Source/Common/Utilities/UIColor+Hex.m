//
//  UIColor+Hex.m
//  Computable
//
//  Created by Karl Traunmüller on 04.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIColor+Hex.h"

@implementation UIColor (Hex)

+ (UIColor *)colorWithHex:(NSInteger)hex alpha:(CGFloat)alpha
{
    NSInteger blue = hex & 0xFF;
    hex >>= 8;
    NSInteger green = hex & 0xFF;
    hex >>= 8;
    NSInteger red = hex & 0xFF;
    hex >>= 8;
    return [UIColor colorWithRed:red/255.0 green:green/255.0 blue:blue/255.0 alpha:alpha];
}

@end
