//
//  HtmlCaptureView.h
//  Computable
//
//  Created by Karl Traunmüller on 14.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface HtmlCaptureView : UIView

- (id)initWithBaseUrl:(NSURL *)baseUrl readyCallback:(NSString *)readyCallback;

- (void)render:(NSString *)html waitForReadyCallback:(BOOL)waitForReadyCallback
   layoutWidth:(CGFloat)layoutWidth
    completion:(void(^)(UIImage *image, NSArray *hyperlinks))completion;
- (void)abortRendering;

@end
