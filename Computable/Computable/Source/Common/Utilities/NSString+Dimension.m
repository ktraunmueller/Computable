//
//  NSString+Dimension.m
//  Computable
//
//  Created by Karl Traunmüller on 08.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSString+Dimension.h"

@implementation NSString (Dimension)

- (CGFloat)layoutHeightForWidth:(CGFloat)width withFont:(UIFont *)font
{
    NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
    paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
    paragraphStyle.alignment = NSTextAlignmentLeft;
    
    NSString *string = self;
    if ([self hasSuffix:@"\n"])
    {
        string = [self stringByAppendingString:@"_"];
    }
    CGRect bounds = [string boundingRectWithSize:CGSizeMake(width, CGFLOAT_MAX)
                                         options:NSStringDrawingUsesLineFragmentOrigin | NSStringDrawingUsesFontLeading
                                      attributes:@{ NSFontAttributeName : font,
                                                    NSParagraphStyleAttributeName : paragraphStyle }
                                         context:nil];
    return CGRectGetHeight(bounds);
}

@end
