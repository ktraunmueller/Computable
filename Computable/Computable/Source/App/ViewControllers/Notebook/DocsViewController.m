//
//  DocsViewController.m
//  Computable
//
//  Created by Karl Traunmüller on 06.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "DocsViewController.h"
#import "../../Core/Python/Editor/Completion.h"
#import "../../Core/Settings/Settings.h"

static const CGFloat kPadding = 4;

@implementation DocsViewController

#pragma mark - UIViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
 
    self.title = _completion.name;
    
    _textView.textContainerInset = UIEdgeInsetsMake(kPadding, kPadding, kPadding, kPadding);
    UIFont *font = [[Settings instance].modernTextFont.boldFont fontWithSize:14];
//    UIFont *font = [[Settings instance].codeFont.regularFont fontWithSize:11];
    _textView.font = font;
    _textView.text = _completion.docString;
}

@end
