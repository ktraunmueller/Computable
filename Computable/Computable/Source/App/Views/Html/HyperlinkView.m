//
//  HyperlinkView.m
//  Computable
//
//  Created by Karl Traunmüller on 28.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "HyperlinkView.h"

@implementation HyperlinkView {
    NSString *_url;
}

- (id)initWithURL:(NSString *)url frame:(CGRect)frame
{
    if (self = [super initWithFrame:frame])
    {
        _url = url;
        
        self.userInteractionEnabled = YES;
        self.backgroundColor = [UIColor clearColor];
        
        UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapped:)];
        [self addGestureRecognizer:tapRecognizer];
    }
    return self;
}

- (void)tapped:(UIGestureRecognizer *)recognizer
{
    if (recognizer.state == UIGestureRecognizerStateEnded)
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:_url]];
    }
}

@end
