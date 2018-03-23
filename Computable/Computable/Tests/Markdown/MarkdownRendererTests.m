//
//  MarkdownRendererTests.m
//  Computable
//
//  Created by Karl Traunmüller on 02.04.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>

#import "../../Source/App/Core/Markdown/Parser/MarkdownParser.h"
#import "../../Source/App/Core/Markdown/Parser/MarkdownRenderer.h"

@interface MarkdownRendererTests : SenTestCase
@end

@implementation MarkdownRendererTests

- (void)testRenderTSP
{
    NSString *md = @"Let's consider the [Traveling Salesperson Problem](http://en.wikipedia.org/wiki/Traveling_salesman_problem): \n\n> *Given a set of cities, and the distances between each pair of cities, find a **tour** of the cities with the minimum total distance.  A **tour** means you start at one city, visit every other city exactly once, and then return to the starting city.*\n\nThis is a well-known [intractable](http://en.wikipedia.org/wiki/Intractability) problem, meaning that there are no efficient solutions that work for a large number of cities.  But we can create an inefficient algorithm that works fine for a small number of cites (about a dozen).  We can also find a *nearly*-shortest tour over thousands of cities. Actually, the fact there is no efficient algorithm is liberating: it means that we can use a very simple, inefficient algorithm and not feel too bad about it. In this exercise we will look at some algorithms for solving the problem.  The most important lesson is how to *think about solving a problem* like this; the details of this particular problem are less important.\n                                     \nLet's think first about the *vocabulary* of this problem: what are the important concepts, and are we sure we understand them well enough to implement the concepts (as data or functions)? We will start to consider possible implementations, in the Python programming language, but won't make definitive choices yet:\n\n- **City**: For the purpose of this exercise, a city is \"atomic\" in the sense that we don't have to know anything about the components or attributes of a city; we don't need to know its name, population, or anything else, except that we do have to know how far it is from other cities.\n- **Cities**: We will need to represent a set of cities; Python's `set` datatype might be appropriate for that.\n- **Distance**: We will need the distance between two cities.  If `A` and `B` are cities, this could be done with a function, `distance(A, B),` or with a dict, `distance[A][B]` or `distance[A, B]`, or with an array if `A` and `B` are integer indexes.  The resulting distance will be a real number (which Python calls a `float`).\n- **Tour**: A tour is an ordered list of cities; Python's `list` or `tuple` datatypes would work.\n- **Total distance**: The total distance for a tour is the sum of the distances of adjacent cities in the tour.  We will probably have a function, `total_distance(tour)`.";
    MarkdownParser *parser = [MarkdownParser new];
    [parser parse:md];
    
    MarkdownRenderer *renderer = [MarkdownRenderer new];
    NSAttributedString *attributedString = [renderer render:parser.tokens];

    STAssertNotNil(attributedString, @"expected render output");
    NSLog(@"%@", attributedString.string);
}

@end
