//
//  UIImage+Overlay.h
//  Computable
//
//  Created by Karl Traunmüller on 14.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIImage (Overlay)

- (UIImage *)imageWithOverlay:(UIImage *)overlay atFrame:(CGRect)overlayFrame;

@end
