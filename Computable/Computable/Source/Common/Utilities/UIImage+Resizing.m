//
//  UIImage+Resizing.m
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIImage+Resizing.h"

@implementation UIImage (Resizing)

- (UIImage *)resizedImageWithSize:(CGSize)newSize
{
    CGFloat screenScale = [[UIScreen mainScreen] scale];
    
    CGRect newRect = CGRectIntegral(CGRectMake(0, 0, newSize.width, newSize.height));
//    NSLog(@"scaling %@ -> %@", NSStringFromCGSize(self.size), NSStringFromCGSize(newRect.size));
    CGImageRef imageRef = self.CGImage;
    
    UIGraphicsBeginImageContextWithOptions(newSize, NO, 0);
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    // Set the quality level to use when rescaling
    CGContextSetInterpolationQuality(context, kCGInterpolationHigh);
    CGAffineTransform flipVertical = CGAffineTransformMake(1, 0, 0, -1, 0, newSize.height);
    
    CGContextConcatCTM(context, flipVertical);
    // Draw into the context; this scales the image
    CGContextDrawImage(context, newRect, imageRef);
    
    // Get the resized image from the context and a UIImage
    CGImageRef newImageRef = CGBitmapContextCreateImage(context);
    UIImage *newImage = [UIImage imageWithCGImage:newImageRef scale:screenScale orientation:self.imageOrientation];
    
    CGImageRelease(newImageRef);
    UIGraphicsEndImageContext();
    
    return newImage;
}

- (UIImage *)resizedImageWithWidth:(CGFloat)width
{
    CGFloat oldWidth = self.size.width;
    CGFloat scaleFactor = width / oldWidth;
    CGFloat newHeight = floor(self.size.height * scaleFactor);
    CGFloat newWidth = oldWidth * scaleFactor;
    return [self resizedImageWithSize:CGSizeMake(newWidth, newHeight)];
}

- (UIImage *)resizedImageWithHeight:(CGFloat)height
{
    CGFloat oldHeight = self.size.height;
    CGFloat scaleFactor = height / oldHeight;
    CGFloat newWidth = floor(self.size.width * scaleFactor);
    CGFloat newHeight = oldHeight * scaleFactor;
    return [self resizedImageWithSize:CGSizeMake(newWidth, newHeight)];
}

@end
