//
//  AboutViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 06.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "AboutViewController.h"
#import "../../Core/Analytics/Analytics.h"

@implementation AboutViewController

#pragma mark - Statics

+ (AboutViewController *)newInstanceFromStoryboard:(UIStoryboard *)storyboard
{
    NSString *identifier = NSStringFromClass([AboutViewController class]);
    AboutViewController *aboutController = [storyboard instantiateViewControllerWithIdentifier:identifier];
    return aboutController;
}

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    [Analytics trackScreenView:@"About"];
    
    _iconView.backgroundColor = [UIColor clearColor];
    _iconView.layer.cornerRadius = 15;
    _iconView.clipsToBounds = YES;
    
    NSBundle *bundle = [NSBundle mainBundle];
    NSString *version = [bundle objectForInfoDictionaryKey:@"CFBundleShortVersionString"];
    _versionLabel.text = [NSString stringWithFormat:NSLocalizedString(@"Version %@", nil), version];
    
    _webView.delegate = self;
    _webView.scrollView.bounces = NO;
    NSString *path = [[NSBundle mainBundle] pathForResource:@"about" ofType:@"html"];
    NSURL *url = [NSURL fileURLWithPath:path];
    [_webView loadRequest:[NSURLRequest requestWithURL:url]];
}

#pragma mark - UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    if ([[request.URL absoluteString] rangeOfString:@"about"].location != NSNotFound)
    {
        return YES;
    }
    [[UIApplication sharedApplication] openURL:request.URL];
    return NO;
}

@end
