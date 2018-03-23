//
//  ComputableTest.h
//  Computable
//
//  Created by Karl Traunmüller on 10.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "SenAsyncTestCase/SenAsyncTestCase.h"

/** a tick is a short period of time (0.1s) */
extern const useconds_t TICK;

/** 1/10th of a tick */
extern const NSTimeInterval EPSILON_T;

#define TICKS_TO_SECONDS(ticks) (ticks * TICK / 1.0e6)

@interface ComputableTest : SenTestCase
@end

@interface ComputableAsyncTest : SenAsyncTestCase
@end
