//
//  HtmlCaptureView.m
//  Computable
//
//  Created by Karl Traunmüller on 14.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "HtmlCaptureView.h"
#import "Hyperlink.h"

typedef void (^RenderResultBlock)(UIImage *image, NSArray *hyperlinks);

@interface RenderRequest : NSObject

@property (nonatomic, copy) NSString *html;
@property (nonatomic, assign) CGFloat layoutWidth;
@property (nonatomic, assign) BOOL waitForReadyCallback;
@property (nonatomic, copy) RenderResultBlock completion;

@end

@implementation RenderRequest
@end

@interface HtmlCaptureView () <UIWebViewDelegate>

@end

@implementation HtmlCaptureView
{
    UIWebView *_webView;
    NSString *_readyCallback;
    NSURL *_baseUrl;
    NSMutableArray *_renderRequests;
    BOOL _rendering;
    CFAbsoluteTime _renderStart;
}

- (id)initWithBaseUrl:(NSURL *)baseUrl readyCallback:(NSString *)readyCallback
{
    if (self = [super initWithFrame:CGRectZero])
    {
        _baseUrl = baseUrl;
        _readyCallback = readyCallback;
        _renderRequests = [NSMutableArray array];
        _webView = [[UIWebView alloc] initWithFrame:self.bounds];
        _webView.delegate = self;
        [self addSubview:_webView];
    }
    return self;
}

- (void)render:(NSString *)html waitForReadyCallback:(BOOL)waitForReadyCallback
   layoutWidth:(CGFloat)layoutWidth
    completion:(void (^)(UIImage *, NSArray *))completion
{
    RenderRequest *request = [RenderRequest new];
    request.html = html;
    request.waitForReadyCallback = waitForReadyCallback;
    request.layoutWidth = layoutWidth;
    request.completion = completion;
    [_renderRequests addObject:request];
    
    if (_rendering)
    {
        return;
    }
    [self renderRequest:request];
}

- (void)abortRendering
{
    [_renderRequests removeAllObjects];
    _rendering = NO;
    [_webView stopLoading];
    _webView.delegate = nil;
    _renderStart = 0;
}

- (void)renderRequest:(RenderRequest *)request
{
    if (_renderStart == 0)
    {
        _renderStart = CFAbsoluteTimeGetCurrent();
    }
    _rendering = YES;
    _webView.delegate = self;
    _webView.frame = CGRectMake(0, 0, request.layoutWidth, 1);
    [_webView loadHTMLString:request.html baseURL:_baseUrl];
}

- (void)completeNextRequest:(BOOL)isReadyCallback
{
    RenderRequest *request = [_renderRequests firstObject];
    if (request == nil)
    {
        return;
    }
    if (!isReadyCallback && request.waitForReadyCallback)
    {
        return;
    }
    [_renderRequests removeObject:request];
    
    CGSize size = [_webView sizeThatFits:CGSizeZero];
    _webView.frame = CGRectMake(0, 0, request.layoutWidth, size.height);
    
    UIGraphicsBeginImageContextWithOptions(CGSizeMake(CGRectGetWidth(_webView.bounds), size.height), YES, 0.0f);
    [_webView.layer renderInContext:UIGraphicsGetCurrentContext()];
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSArray *encodedLinks = [[_webView stringByEvaluatingJavaScriptFromString:@"clientRectsOfLinks()"] componentsSeparatedByString:@"\n"];
    NSArray *decodedLinnks = [self decodeLinks:encodedLinks];
    
    _rendering = NO;
    
    if (request.completion)
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            request.completion(image, decodedLinnks);
        });
    }
    
    RenderRequest *nextRequest = [_renderRequests firstObject];
    if (nextRequest)
    {
        [self renderRequest:nextRequest];
    }
    else
    {
        NSLog(@"rendering Markdown took %d ms", (int)((CFAbsoluteTimeGetCurrent() - _renderStart) * 1000));
    }
}

- (NSArray *)decodeLinks:(NSArray *)encodedLinks
{
    if (encodedLinks == nil)
    {
        return nil;
    }
    NSMutableArray *links = [NSMutableArray array];
    for (NSString *encodedLink in encodedLinks)
    {
        if (encodedLink.length == 0)
        {
            continue;
        }
        NSRange pipe = [encodedLink rangeOfString:@"|"];
        if (pipe.location == NSNotFound)
        {
            continue;
        }
        NSString *url = [encodedLink substringToIndex:pipe.location];
        NSArray *rects = [[encodedLink substringFromIndex:pipe.location + 1] componentsSeparatedByString:@";"];
        NSMutableArray *clientRects = [NSMutableArray array];
        for (NSString *rect in rects)
        {
            if (rect.length == 0)
            {
                continue;
            }
            NSArray *components = [rect componentsSeparatedByString:@","];
            if ([components count] < 4)
            {
                continue;
            }
            CGFloat x = [components[0] floatValue];
            CGFloat y = [components[1] floatValue];
            CGFloat w = [components[2] floatValue];
            CGFloat h = [components[3] floatValue];
            CGRect rect = CGRectMake(x, y, w, h);
            [clientRects addObject:[NSValue valueWithCGRect:rect]];
        }
        Hyperlink *link = [Hyperlink new];
        link.url = url;
        link.clientRects = clientRects;
        [links addObject:link];
    }
    return [links count] > 0 ? links : nil;
}

#pragma mark - UIWebViewDelegate

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSString *url = [request.URL absoluteString];
    if (_readyCallback && [url rangeOfString:_readyCallback].location != NSNotFound)
    {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.01 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            [self completeNextRequest:YES];
        });
        return NO;
    }
    return YES;
}

- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.01 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        [self completeNextRequest:NO];
    });
}

@end
