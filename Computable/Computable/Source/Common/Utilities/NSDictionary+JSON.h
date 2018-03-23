//
//  NSDictionary+JSON.h
//  PyBooks-iOS
//
//  Created by Karl Traunmüller on 04.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSDictionary (JSON)

- (NSString *)asJsonString;
- (void)writeToFile:(NSString *)path;

@end
