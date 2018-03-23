//
//  UIImage+Base64.m
//  Computable
//
//  Created by Karl Traunmüller on 14.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "UIImage+Base64.h"

@implementation UIImage (Base64)

+ (UIImage *)imageFromBase64String:(NSString *)base64String
{
    return [UIImage imageFromBase64String:base64String useDeviceScale:YES];
}

+ (UIImage *)imageFromBase64String:(NSString *)base64String useDeviceScale:(BOOL)useDeviceScale
{
    if (base64String == nil)
    {
        return nil;
    }
    //    base64String = [@"data:image/png;base64," stringByAppendingString:base64String];
    NSData *data = [[NSData alloc] initWithBase64EncodedString:base64String options:NSDataBase64DecodingIgnoreUnknownCharacters];
    UIImage *image = [UIImage imageWithData:data];
    if (useDeviceScale)
    {
        image = [UIImage imageWithCGImage:[image CGImage]
                                    scale:[UIScreen mainScreen].scale
                              orientation:UIImageOrientationUp];
    }
    return image;
}

- (NSString *)base64String
{
    NSData *data = UIImagePNGRepresentation(self);
    return [data base64EncodedStringWithOptions:0];
}

@end
