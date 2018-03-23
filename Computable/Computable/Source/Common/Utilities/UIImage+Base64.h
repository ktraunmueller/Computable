//
//  UIImage+Base64.h
//  Computable
//
//  Created by Karl Traunmüller on 14.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (Base64)

+ (UIImage *)imageFromBase64String:(NSString *)base64;
+ (UIImage *)imageFromBase64String:(NSString *)base64 useDeviceScale:(BOOL)useDeviceScale;

- (NSString *)base64String;

@end
