//
//  NSAttributedString+Dimension.m
//  Computable
//
//  Created by Karl Traunmüller on 21.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSAttributedString+Dimension.h"

@implementation NSAttributedString (Dimension)

- (CGFloat)layoutHeightForWidth:(CGFloat)width withFont:(UIFont *)font
{
    NSMutableAttributedString *string = [self mutableCopy];
    if ([string.string hasSuffix:@"\n"])
    {
        [string appendAttributedString:[[NSAttributedString alloc] initWithString:@"_"]];
    }
    CGSize size = CGSizeMake(width, MAXFLOAT);
    CGRect rect = [string boundingRectWithSize:size
                                       options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading
                                       context:nil];
    return CGRectGetHeight(rect);
}

@end
