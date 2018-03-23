//
//  UIImage+Overlay.m
//  Computable
//
//  Created by Karl Traunmüller on 14.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIImage+Overlay.h"
#import "UIImage+Resizing.h"

@implementation UIImage (Overlay)

- (UIImage *)imageWithOverlay:(UIImage *)overlay atFrame:(CGRect)overlayFrame
{
    CGImageRef imageRef = self.CGImage;
    
    UIGraphicsBeginImageContextWithOptions(self.size, NO, 0);
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetInterpolationQuality(context, kCGInterpolationHigh);
    CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, self.size.height);
    CGContextConcatCTM(context, flipVertical);
    CGContextDrawImage(context, CGRectMake(0, 0, self.size.width, self.size.height), imageRef);
    CGContextDrawImage(context, overlayFrame, overlay.CGImage);
    
    CGImageRef newImageRef = CGBitmapContextCreateImage(context);
    UIImage *newImage = [UIImage imageWithCGImage:newImageRef scale:[UIScreen mainScreen].scale orientation:self.imageOrientation];
    CGImageRelease(newImageRef);
    UIGraphicsEndImageContext();
    
    return newImage;
}

@end
