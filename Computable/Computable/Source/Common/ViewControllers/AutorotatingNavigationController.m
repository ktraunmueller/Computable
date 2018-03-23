//
//  AutorotatingNavigationController.m
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "AutorotatingNavigationController.h"

@implementation AutorotatingNavigationController

#pragma mark - Autorotation

- (BOOL)shouldAutorotate {
    return YES;
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskAll;
}

@end
