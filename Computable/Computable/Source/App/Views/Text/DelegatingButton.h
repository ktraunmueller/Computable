//
//  DelegatingButton.h
//  Computable
//
//  Created by Karl Traunmüller on 01.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DelegatingButton : UIButton

@property (nonatomic, assign) SEL action;
@property (nonatomic, strong) UIColor *normalColor; // defaults to white
@property (nonatomic, strong) UIColor *highlightedColor;

@end
