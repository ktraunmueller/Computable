//
//  NSString+LineNumbers.m
//  Computable
//
//  Created by Karl Traunmüller on 05.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NSString+Location.h"

@implementation NSString (Location)

- (TextLocation)locationForRange:(NSRange)range
{
    NSRange lineRange = [self lineRangeForRange:range];
    TextLocation location;
    location.line = NSNotFound;
    location.column = NSNotFound;
    if (lineRange.location != NSNotFound)
    {
        location.line = 0;
        location.column = range.location - lineRange.location;
        while (lineRange.location != NSNotFound && lineRange.location > 0)
        {
            location.line++;
            lineRange = [self lineRangeForRange:NSMakeRange(lineRange.location - 1, 0)];
        }
    }
    return location;
}

@end
