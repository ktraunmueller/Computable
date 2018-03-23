//
//  UIView+LayoutDiagnostics.h
//  sleep
//
//  Created by Karl Traunmüller on 30.04.14.
//  Copyright (c) 2014 Runtastic. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol LayoutDiagnostics <NSObject>

- (void)enableLayoutDiagnostics:(BOOL)enable;
- (void)enableLayoutDiagnostics:(BOOL)enable borderColor:(UIColor *)borderColor;

- (void)addMarkerAtPoint:(CGPoint)markerPosition color:(UIColor *)color;

@end


@interface UIView (LayoutDiagnostics) <LayoutDiagnostics>
@end

@interface UIButton (LayoutDiagnostics) <LayoutDiagnostics>
@end