//
//  AnsiParser.h
//  Computable
//
//  Created by Karl Traunmüller on 08.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AnsiParser : NSObject

- (NSAttributedString *)parse:(const char *)ansi;

@end
