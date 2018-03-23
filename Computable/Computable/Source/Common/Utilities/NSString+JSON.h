//
//  NSString+JSON.h
//  Computable
//
//  Created by Karl Traunmüller on 06.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (JSON)

- (NSMutableDictionary *)asJsonDictionary;

@end
