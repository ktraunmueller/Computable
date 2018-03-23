//
//  VideoPlayerView.h
//  Computable
//
//  Created by Karl Traunmüller on 15.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface VideoPlayerView : UIView

@property (nonatomic, readonly) BOOL isVideoLoaded;

- (void)loadVideo:(NSString *)path;

- (void)rewind;
- (void)start;
- (void)stop;

@end
