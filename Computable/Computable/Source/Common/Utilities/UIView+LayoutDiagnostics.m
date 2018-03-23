//
//  UIView+LayoutDiagnostics.m
//  sleep
//
//  Created by Karl Traunmüller on 30.04.14.
//  Copyright (c) 2014 Runtastic. All rights reserved.
//

#import "UIView+LayoutDiagnostics.h"

@implementation UIView (LayoutDiagnostics)

- (void)enableLayoutDiagnostics:(BOOL)enable {
    [self enableLayoutDiagnostics:enable borderColor:[UIColor greenColor]];
}

- (void)enableLayoutDiagnostics:(BOOL)enable borderColor:(UIColor *)borderColor {
    self.backgroundColor = enable ? [borderColor colorWithAlphaComponent:0.1] : [UIColor clearColor];
    self.layer.borderWidth = enable ? 1 : 0;
    self.layer.borderColor = enable ? [borderColor CGColor] : [[UIColor clearColor] CGColor];
    
    for (UIView *subview in self.subviews) {
        [subview enableLayoutDiagnostics:enable borderColor:borderColor];
    }
}

- (void)addMarkerAtPoint:(CGPoint)markerPosition color:(UIColor *)color {
    CGRect frame = CGRectMake(markerPosition.x - 5, markerPosition.y - 5, 10, 10);
    UIView *marker = [[UIView alloc] initWithFrame:frame];
    marker.layer.backgroundColor = [color colorWithAlphaComponent:0.5].CGColor;
    marker.layer.borderColor = color.CGColor;
    marker.layer.borderWidth = 1;
    marker.layer.opacity = 0.5;
    [self addSubview:marker];
}

@end

@implementation UIButton (LayoutDiagnostics)

- (void)enableLayoutDiagnostics:(BOOL)enable borderColor:(UIColor *)borderColor {
    [super enableLayoutDiagnostics:enable borderColor:borderColor];
    
    [self.titleLabel enableLayoutDiagnostics:enable borderColor:borderColor];
    [self.imageView enableLayoutDiagnostics:enable borderColor:borderColor];
}

@end
