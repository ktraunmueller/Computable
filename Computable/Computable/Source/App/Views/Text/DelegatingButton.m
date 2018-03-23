//
//  DelegatingButton.m
//  Computable
//
//  Created by Karl Traunmüller on 01.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "DelegatingButton.h"

@implementation DelegatingButton

- (void)setHighlighted:(BOOL)highlighted
{
    [super setHighlighted:highlighted];
    
    self.backgroundColor = highlighted ? (_highlightedColor ? _highlightedColor : [UIColor colorWithRed:208/255.0 green:209/255.0 blue:213/255.0 alpha:1.0]) : (_normalColor ? _normalColor : [UIColor whiteColor]);
    self.titleLabel.alpha = 1;
}

@end
