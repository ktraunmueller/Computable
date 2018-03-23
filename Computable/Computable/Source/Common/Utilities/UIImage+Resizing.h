//
//  UIImage+Resizing.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (Resizing)

- (UIImage *)resizedImageWithSize:(CGSize)newSize;
- (UIImage *)resizedImageWithWidth:(CGFloat)width;
- (UIImage *)resizedImageWithHeight:(CGFloat)height;

@end
