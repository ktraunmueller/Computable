//
//  FontSetting.m
//  Computable
//
//  Created by Karl Traunmüller on 04.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "FontSetting.h"

@implementation FontSetting

- (UIFont *)regularFont
{
    return [UIFont fontWithName:self.regularFontName size:self.size];
}

- (UIFont *)boldFont
{
    return [UIFont fontWithName:self.boldFontName size:self.size];
}

- (UIFont *)italicFont
{
    return [UIFont fontWithName:self.italicFontName size:self.size];
}

- (UIFont *)boldItalicFont
{
    return [UIFont fontWithName:self.boldItalicFontName size:self.size];
}

@end
