//
//  UIColor+AnsiColors.h
//  Computable
//
//  Created by Karl Traunmüller on 08.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

// see http://en.wikipedia.org/wiki/ANSI_escape_code#graphics
typedef NS_ENUM(NSUInteger, SGRParameter)
{
    SGRParameterResetNormal = 0,
    SGRParameterSetTextColor = 30,
    SGRParameterSetBackgroundColor = 40,
};

@interface UIColor (AnsiColors)

+ (UIColor *)ansiColor:(SGRParameter)color;

@end
