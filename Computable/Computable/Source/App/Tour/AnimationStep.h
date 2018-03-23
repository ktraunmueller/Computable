//
//  AnimationStep.h
//  Computable
//
//  Created by Karl Traunmüller on 13.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AnimationStep : NSObject

@property (nonatomic, assign) NSTimeInterval duration;
@property (nonatomic, assign) NSTimeInterval delay;
@property (nonatomic, assign) UIViewAnimationOptions options;
@property (nonatomic, assign) CGAffineTransform transform;
@property (nonatomic, strong) UIImage *image;

+ (NSTimeInterval)totalDuration:(NSArray *)animationSteps;

@end
