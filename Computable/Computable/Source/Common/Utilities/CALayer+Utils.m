//
//  CALayer+Utils.m
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "CALayer+Utils.h"

@implementation CALayer (Utils)

- (UIImage *)captureViewIntoImage
{
    UIGraphicsBeginImageContextWithOptions(self.bounds.size, NO, 0.0);
    [self renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}

@end
