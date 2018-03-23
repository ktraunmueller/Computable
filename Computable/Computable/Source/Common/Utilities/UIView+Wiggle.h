//
//  UIView+Wiggle.h
//  Computable
//
//  Created by Karl Traunmüller on 18.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UIView (Wiggle)

- (void)startWiggleAnimation;
- (void)stopWiggleAnimation;
- (BOOL)isWiggleAnimationRunning;

@end
