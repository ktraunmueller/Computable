//
//  NotebookBrowserLayout.m
//  Computable
//
//  Created by Karl Traunmüller on 16.03.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "NotebookBrowserLayout.h"

@implementation NotebookBrowserLayout

// Source: http://blog.radi.ws/post/32905838158/sticky-headers-for-uicollectionview-using
- (NSArray *)layoutAttributesForElementsInRect:(CGRect)rect
{
    NSMutableArray *result = [[super layoutAttributesForElementsInRect:rect] mutableCopy];
    UICollectionView *collectionView = self.collectionView;
    CGPoint contentOffset = collectionView.contentOffset;
    
    NSMutableIndexSet *sections = [NSMutableIndexSet indexSet];
    for (UICollectionViewLayoutAttributes *layoutAttributes in result)
    {
        if (layoutAttributes.representedElementCategory == UICollectionElementCategoryCell)
        {
            [sections addIndex:layoutAttributes.indexPath.section];
        }
    }
    for (UICollectionViewLayoutAttributes *layoutAttributes in result)
    {
        if ([layoutAttributes.representedElementKind isEqualToString:UICollectionElementKindSectionHeader])
        {
            [sections removeIndex:layoutAttributes.indexPath.section];
        }
    }
    
    [sections enumerateIndexesUsingBlock:^(NSUInteger idx, BOOL *stop)
     {
         NSIndexPath *indexPath = [NSIndexPath indexPathForItem:0 inSection:idx];
         UICollectionViewLayoutAttributes *layoutAttributes = [self layoutAttributesForSupplementaryViewOfKind:UICollectionElementKindSectionHeader
                                                                                                   atIndexPath:indexPath];
         [result addObject:layoutAttributes];
     }];
    
    for (UICollectionViewLayoutAttributes *layoutAttributes in result)
    {
        if ([layoutAttributes.representedElementKind isEqualToString:UICollectionElementKindSectionHeader])
        {
            NSInteger section = layoutAttributes.indexPath.section;
            NSInteger numberOfItemsInSection = [collectionView numberOfItemsInSection:section];
            
            NSIndexPath *firstObjectIndexPath = [NSIndexPath indexPathForItem:0 inSection:section];
            NSIndexPath *lastObjectIndexPath = [NSIndexPath indexPathForItem:MAX(0, (numberOfItemsInSection - 1)) inSection:section];
            
            BOOL cellsExist = numberOfItemsInSection > 0;
            UICollectionViewLayoutAttributes *firstObjectAttrs = nil;
            UICollectionViewLayoutAttributes *lastObjectAttrs = nil;
            if (cellsExist)
            {
                // use cell data if items exist
                firstObjectAttrs = [self layoutAttributesForItemAtIndexPath:firstObjectIndexPath];
                lastObjectAttrs = [self layoutAttributesForItemAtIndexPath:lastObjectIndexPath];
            }
            else
            {
                // else use the header and footer
                firstObjectAttrs = [self layoutAttributesForSupplementaryViewOfKind:UICollectionElementKindSectionHeader
                                                                        atIndexPath:firstObjectIndexPath];
                lastObjectAttrs = [self layoutAttributesForSupplementaryViewOfKind:UICollectionElementKindSectionFooter
                                                                       atIndexPath:lastObjectIndexPath];
                
            }
            
            CGFloat topHeaderHeight = cellsExist ? CGRectGetHeight(layoutAttributes.frame) : 0;
            CGRect frameWithEdgeInsets = UIEdgeInsetsInsetRect(layoutAttributes.frame, collectionView.contentInset);
            
            CGPoint origin = frameWithEdgeInsets.origin;
            origin.y = fminf(fmaxf(contentOffset.y + collectionView.contentInset.top,
                               CGRectGetMinY(firstObjectAttrs.frame) - topHeaderHeight),
                           CGRectGetMaxY(lastObjectAttrs.frame) - 20);
            
            layoutAttributes.zIndex = 1024 - section;
            layoutAttributes.frame = (CGRect){
                .origin = origin,
                .size = layoutAttributes.frame.size
            };
        }
    }
    
    return result;
}

- (BOOL)shouldInvalidateLayoutForBoundsChange:(CGRect)newBound
{
    return YES;
}

@end
