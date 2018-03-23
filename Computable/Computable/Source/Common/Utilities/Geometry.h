//
//  Geometry.h
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface Geometry : NSObject

+ (CGPoint)add:(CGPoint)vector to:(CGPoint)point;
+ (CGPoint)subtract:(CGPoint)vector from:(CGPoint)point;
+ (CGPoint)rotatePoint:(CGPoint)p by:(CGFloat)theta;
+ (CGPoint)randomlyOffset:(CGPoint)point by:(CGFloat)maxDistance;

@end
