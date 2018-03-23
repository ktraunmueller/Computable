//
//  Cue.h
//  Computable
//
//  Created by Karl Traunmüller on 11.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(NSUInteger, CueOptions)
{
    CueOptionNone       = 0,
    CueOptionTop        = 1 << 0,
    CueOptionHOffset    = 1 << 1,
    CueOptionScale      = 1 << 2,
    CueOptionAlpha      = 1 << 3,
    CueOptionAll = CueOptionTop | CueOptionHOffset | CueOptionScale | CueOptionAlpha
};

@class Cue;

extern Cue *kNopCue;

@interface Cue : NSObject

@property (nonatomic, assign) CGFloat top;
@property (nonatomic, assign) CGFloat hOffset;
@property (nonatomic, assign) CGFloat scale;
@property (nonatomic, assign) CGFloat alpha;

+ (Cue *)cueWithTop:(CGFloat)top hOffset:(CGFloat)hOffset scale:(CGFloat)scale alpha:(CGFloat)alpha;

@end
