//
//  NSString+Location.h
//  Computable
//
//  Created by Karl Traunmüller on 05.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct _TextLocation {
    NSUInteger line;
    NSUInteger column;
} TextLocation;

@interface NSString (Location)

- (TextLocation)locationForRange:(NSRange)range;

@end
