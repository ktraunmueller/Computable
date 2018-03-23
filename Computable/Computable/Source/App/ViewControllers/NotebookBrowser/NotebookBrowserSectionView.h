//
//  NotebookBrowserSectionView.h
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface NotebookBrowserSectionView : UICollectionReusableView

@property (nonatomic, readwrite) IBOutlet UILabel *titleLabel;
@property (nonatomic, readwrite) IBOutlet UIView *separatorView;
@property (nonatomic, readwrite) IBOutlet NSLayoutConstraint *separatorHeightConstraint;

@end
