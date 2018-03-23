//
//  UIColor+Random.m
//  scrollpager
//
//  Created by Karl Traunmüller on 03.02.14.
//  Copyright (c) 2014 Runtastic. All rights reserved.
//

#import "UIColor+Random.h"

@implementation UIColor (Random)

+ (UIColor *)randomColor {
    int red = arc4random() % 256;
    int green = arc4random() % 256;
    int blue = arc4random() % 256;
    return [UIColor colorWithRed:red / 255.0 green:green / 255.0 blue:blue / 255.0 alpha:1.0];
}

@end
