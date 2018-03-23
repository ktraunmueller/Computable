//
//  PythonEditor.h
//  Computable
//
//  Created by Karl Traunmüller on 05.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "Completion.h"

@interface PythonEditor : NSObject

+ (void)completionsForSource:(NSString *)source
                      atLine:(NSUInteger)line   // zero-based
                      column:(NSInteger)column  // zero-based
                      result:(void (^)(NSArray *completions))result;


@end
