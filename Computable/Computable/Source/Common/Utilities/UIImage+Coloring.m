//
//  UIImage+Coloring.m
//  Computable
//
//  Created by Karl Traunmüller on 17.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIImage+Coloring.h"

@implementation UIImage (Coloring)

- (UIImage *)colorWith:(UIColor *)color
{
    UIGraphicsBeginImageContextWithOptions(self.size, NO, self.scale);
    
    [self drawInRect:CGRectMake(0, 0, self.size.width, self.size.height)];
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetBlendMode(context, kCGBlendModeSourceIn);
    
    CGContextSetFillColorWithColor(context, color.CGColor);
    CGContextFillRect(context, CGRectMake(0, 0, self.size.width, self.size.height));
    
    UIImage *coloredImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return coloredImage;
}

@end
