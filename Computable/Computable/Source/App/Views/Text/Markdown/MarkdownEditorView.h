//
//  MarkdownEditorView.h
//  Computable
//
//  Created by Karl Traunmüller on 09.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "MarkdownInputAccessoryView.h"
#import "../EditorView.h"

@interface MarkdownEditorView : EditorView <MarkdownInputAccessoryDelegate>

+ (MarkdownEditorView *)newInstanceWithFrame:(CGRect)frame;

@end