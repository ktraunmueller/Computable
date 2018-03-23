//
//  DocsViewController.h
//  Computable
//
//  Created by Karl Traunmüller on 06.08.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Completion;

@interface DocsViewController : UIViewController
{
@private
    __weak IBOutlet UITextView *_textView;
}

@property (nonatomic, strong) Completion *completion;

@end
