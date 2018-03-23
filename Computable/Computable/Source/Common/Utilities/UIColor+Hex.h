//
//  UIColor+Hex.h
//  Computable
//
//  Created by Karl Traunmüller on 04.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIColor (Hex)

+ (UIColor *)colorWithHex:(NSInteger)hex alpha:(CGFloat)alpha;

@end
