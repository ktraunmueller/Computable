//
//  NSString+PythonRuntime.h
//  Computable
//
//  Created by Karl Traunmüller on 18.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (PythonRuntime)

- (NSString *)stringByStrippingPythonPath;

@end
