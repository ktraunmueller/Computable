//
//  AboutViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 06.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface AboutViewController : UIViewController <UIWebViewDelegate>
{
@private
    __weak IBOutlet UIImageView *_iconView;
    __weak IBOutlet UILabel *_versionLabel;
    __weak IBOutlet UIWebView *_webView;
}

+ (AboutViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard;

@end
