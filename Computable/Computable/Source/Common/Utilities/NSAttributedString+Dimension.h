//
//  NSAttributedString+Dimension.h
//  Computable
//
//  Created by Karl Traunmüller on 21.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSAttributedString (Dimension)

- (CGFloat)layoutHeightForWidth:(CGFloat)width withFont:(UIFont *)font;

@end
