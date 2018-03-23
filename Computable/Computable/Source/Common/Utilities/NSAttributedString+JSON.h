//
//  NSAttributedString+JSON.h
//  Computable
//
//  Created by Karl Traunmüller on 08.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSAttributedString (JSON)

- (NSMutableDictionary *)asJsonDictionary;

@end
