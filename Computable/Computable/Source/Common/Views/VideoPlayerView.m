//
//  VideoPlayerView.m
//  Computable
//
//  Created by Karl Traunmüller on 15.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "VideoPlayerView.h"

#import <AVFoundation/AVFoundation.h>

@implementation VideoPlayerView
{
    AVPlayerLayer *_playerLayer;
    AVPlayerItem *_playerItem;
    AVPlayer *_player;
}

#pragma mark - Properties

- (BOOL)isVideoLoaded
{
    return _player != nil;
}

#pragma mark - VideoPlayerView

- (void)setup
{
    _playerLayer = [AVPlayerLayer new];
    _playerLayer.bounds = self.bounds;
    _playerLayer.position = self.center;
//    _playerLayer.contentsGravity = kCAGravityCenter;
    [self.layer addSublayer:_playerLayer];
//    self.layer.masksToBounds = YES;
}

- (void)loadVideo:(NSString *)path
{
    if (path == nil)
    {
        return;
    }
    NSURL *url = [NSURL fileURLWithPath:path];
    _playerItem = [AVPlayerItem playerItemWithURL:url];
    
    _player = [AVPlayer playerWithPlayerItem:_playerItem];
    _playerLayer.player = _player;
}

- (void)rewind
{
    [_player seekToTime:kCMTimeZero];   
}

- (void)start
{
    [_player play];
}

- (void)stop
{
    if (_player.rate > 0)
    {
        [_player pause];
    }
}

#pragma mark - UIView

- (id)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame])
    {
        [self setup];
    }
    return self;
}

- (id)initWithCoder:(NSCoder *)coder
{
    if (self = [super initWithCoder:coder])
    {
        [self setup];
    }
    return self;
}

- (void)layoutSublayersOfLayer:(CALayer *)layer
{
    if (layer == self.layer)
    {
        _playerLayer.bounds = self.bounds;
        _playerLayer.position = self.center;
    }
}

@end
