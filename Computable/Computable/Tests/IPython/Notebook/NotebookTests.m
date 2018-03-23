//
//  NotebookTests.m
//  Computable
//
//  Created by Karl Traunmüller on 10.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "ComputableTest.h"
#import "../../../Source/App/Core/IPython/IPythonApplication.h"
#import "../../../Source/App/Core/IPython/Notebook/Notebook.h"
#import "../../../Source/App/Core/IPython/Kernel/Kernel.h"
#import "../../../Source/App/Core/Python/Runtime/PythonRuntime.h"

@interface NotebookTests : ComputableAsyncTest
@end

@implementation NotebookTests

- (void)runTest:(dispatch_block_t)block
{
    dispatch_async(dispatch_get_main_queue(), ^{
        [IPythonApplication prepare];
        [[PythonRuntime instance] setup:^{
            block();
            [IPythonApplication stop:^{
                self.finished = YES;
            }];
        }];
    });
}

#pragma mark - Basic Tests

- (void)testNewNotebook
{
    self.timeoutInterval = 10;
    
    [self runTest:^{
        [IPythonApplication newNotebook:^(Notebook *notebook) {
            STAssertNotNil(notebook, @"notebook is nil");
            self.finished = YES;
        }];
    }];
}

//- (void)testImportSys
//{
//    self.timeoutInterval = 10;
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:@"import sys"
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testExecuteWithOutput
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"x = 1\n"
//    @"print x";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                       STAssertEqualObjects(response.streamData, @"1\n", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testSyntaxError
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"x = 2"
//    @"print x";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       STAssertNotNil(response.input, @"expected input");
//                       return;
//                   }
//                   STAssertTrue(response.isError, @"expected error response");
//                   STAssertEqualObjects(response.errorName, @"SyntaxError", @"expected syntax error");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testChainedExecutions
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"x = 1\n"
//    @"print x";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                       STAssertEqualObjects(response.streamData, @"1\n", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//
//                   NSString *code =
//                   @"x = 2\n"
//                   @"print x";
//                   [notebook execute:code
//                          completion:^(Message *request, Message *response) {
//                              if (response.isInput)
//                              {
//                                  STAssertTrue(response.executionCount > 0, @"expected execution count");
//                                  return;
//                              }
//                              if (response.isStream)
//                              {
//                                  STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                                  STAssertEqualObjects(response.streamData, @"2\n", @"unexpected output");
//                                  return;
//                              }
//                              STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                              STAssertTrue(response.status == StatusOk, @"unexpected status");
//                              [notebook stop:^{
//                                  self.finished = YES;
//                              }];
//                          }];
//               }];
//    }];
//}
//
//- (void)testBuiltinRandom
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"import random\n"
//    @"r = random.randrange(5, 10)\n"
//    @"print r";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                       NSInteger result = [response.streamData integerValue];
//                       STAssertTrue(result >= 5 && result <= 10, @"unexpected result");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//#pragma mark - NumPy
//
//// http://nbviewer.ipython.org/github/jrjohansson/scientific-python-lectures/blob/master/Lecture-2-Numpy.ipynb
//
//- (void)testImportNumPy
//{
//    self.timeoutInterval = 10;
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:@"import numpy"
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyArray
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import *\n"
//    @"v = array([1,2,3,4])\n"
//    @"print v";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                       STAssertEqualObjects(response.streamData, @"[1 2 3 4]\n", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyLinSpace
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import *\n"
//    @"x = linspace(0, 10, 25)\n"
//    @"print x";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                       STAssertEqualObjects(response.streamData,
//                                            @"[  0.           0.41666667   0.83333333   1.25         1.66666667\n"
//                                            @"   2.08333333   2.5          2.91666667   3.33333333   3.75         4.16666667\n"
//                                            @"   4.58333333   5.           5.41666667   5.83333333   6.25         6.66666667\n"
//                                            @"   7.08333333   7.5          7.91666667   8.33333333   8.75         9.16666667\n"
//                                            @"   9.58333333  10.        ]\n", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyRandom
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import random\n"
//    @"random.rand(5, 5)";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertTrue([response.output hasPrefix:@"array([["], @"unexpected output");
//                       STAssertTrue([response.output hasSuffix:@"]])"], @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyScalarArrayOperations
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import *\n"
//    @"v1 = arange(0, 5)\n"
//    @"v1 * 2";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertEqualObjects(response.output, @"array([0, 2, 4, 6, 8])", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyMatrixDiag
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import *\n"
//    @"A = array([[n + m * 10 for n in range(5)] for m in range(5)])\n"
//    @"diag(A)";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertEqualObjects(response.output, @"array([ 0, 11, 22, 33, 44])", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyMatrixDot
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import *\n"
//    @"A = array([[n + m * 10 for n in range(5)] for m in range(5)])\n"
//    @"dot(A, A)";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertEqualObjects(response.output,
//                                            @"array([[ 300,  310,  320,  330,  340],\n"
//                                            @"       [1300, 1360, 1420, 1480, 1540],\n"
//                                            @"       [2300, 2410, 2520, 2630, 2740],\n"
//                                            @"       [3300, 3460, 3620, 3780, 3940],\n"
//                                            @"       [4300, 4510, 4720, 4930, 5140]])", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyMatrixComplexConjugate
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import *\n"
//    @"C = matrix([[1j, 2j], [3j, 4j]])\n"
//    @"conjugate(C)";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertEqualObjects(response.output,
//                                            @"matrix([[ 0.-1.j,  0.-2.j],\n"
//                                            @"        [ 0.-3.j,  0.-4.j]])", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testNumpyMatrixComplexInverse
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from numpy import *\n"
//    @"from numpy.dual import inv\n"
//    @"C = matrix([[1j, 2j], [3j, 4j]])\n"
//    @"inv(C)";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertEqualObjects(response.output,
//                                            @"matrix([[ 0.+2.j ,  0.-1.j ],\n"
//                                            @"        [ 0.-1.5j,  0.+0.5j]])", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//// TODO get as much as possible of the test suite running
//- (void)_testNumPyRunFullTestSuite
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"import numpy as np\n"
//    @"np.test('full')";
//
//    __block BOOL runningUnitTestsMessageReceived = NO;
//    __block BOOL numpyInstalledMessageReceived = NO;
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (!runningUnitTestsMessageReceived)
//                   {
//                       STAssertTrue(response.isStream, @"expected stream message");
//                       STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                       STAssertEqualObjects(response.streamData, @"Running unit tests for numpy\nNumPy version 1.8.0", @"unexpected output");
//                       runningUnitTestsMessageReceived = YES;
//                       return;
//                   }
//                   if (!numpyInstalledMessageReceived)
//                   {
//                       STAssertTrue(response.isStream, @"expected stream message");
//                       STAssertTrue(response.streamType == StreamTypeStdOut, @"expected stdout");
//                       STAssertTrue([response.streamData hasPrefix:@"\nNumPy is installed"], @"unexpected output");
//                       STAssertTrue([response.streamData hasSuffix:@"nose version 1.3.0\n"], @"unexpected output");
//                       numpyInstalledMessageReceived = YES;
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       // <nose.result.TextTestResult run=0 errors=0 failures=0>
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       NSString *prefix = @"\n----------------------------------------------------------------------\nRan ";
//                       STAssertTrue([response.streamData hasPrefix:prefix], @"unexpected output");
//                       if ([response.streamData hasPrefix:prefix])
//                       {
//                           NSString *remainder = [response.streamData length] > [prefix length] ? [response.streamData substringFromIndex:[prefix length]] : nil;
//                           NSInteger numberOfTests = [remainder integerValue];
//                           STAssertEquals(numberOfTests, 0, @"unexpected number of tests");
//                       }
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//#pragma mark - SciPy
//
//// http://nbviewer.ipython.org/github/jrjohansson/scientific-python-lectures/tree/master/
//// http://scipy-lectures.github.io
//
//- (void)testImportSciPy
//{
//    self.timeoutInterval = 10;
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:@"import scipy"
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testSciPyLinalgMatrixInvert
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"import numpy as np\n"
//    @"from scipy.linalg import *\n"
//    @"A = np.array([[1,2],[3,4]])\n"
//    @"inv(A)";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertEqualObjects(response.output,
//                                            @"array([[-2. ,  1. ],\n"
//                                            @"       [ 1.5, -0.5]])", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testSciPySpecialBesselFirstKind
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from scipy.special import jn, yn, jn_zeros, yn_zeros\n"
//    @"n = 0\n"
//    @"x = 0.0\n"
//    @"print 'J_%d(%f) = %f' % (n, x, jn(n, x))";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       STAssertEqualObjects(response.streamData,
//                                            @"J_0(0.000000) = 1.000000\n", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testSciPySpecialBesselZeros
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from scipy.special import jn, yn, jn_zeros, yn_zeros\n"
//    @"n = 0\n"
//    @"m = 4\n"
//    @"jn_zeros(n, m)";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isOutput)
//                   {
//                       STAssertEqualObjects(response.output,
//                                            @"array([  2.40482556,   5.52007811,   8.65372791,  11.79153444])", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}
//
//- (void)testSciPyNumericalIntegrationQuad
//{
//    self.timeoutInterval = 10;
//
//    NSString *code =
//    @"from scipy.integrate import quad, dblquad, tplquad\n"
//    @"def f(x):\n"
//    @"  return x\n"
//    @"val, abserr = quad(f, 0, 1)\n"
//    @"print 'integral value =', val, ', absolute error =', abserr";
//
//    Notebook *notebook = [Notebook newInstance];
//    [notebook start:^{
//        [notebook execute:code
//               completion:^(Message *request, Message *response) {
//                   if (response.isInput)
//                   {
//                       STAssertTrue(response.executionCount > 0, @"expected execution count");
//                       return;
//                   }
//                   if (response.isStream)
//                   {
//                       STAssertEqualObjects(response.streamData,
//                                            @"integral value = 0.5 , absolute error = 5.55111512313e-15\n", @"unexpected output");
//                       return;
//                   }
//                   STAssertTrue(response.type == MessageTypeExecuteReply, @"expected execute reply");
//                   STAssertTrue(response.status == StatusOk, @"unexpected status");
//                   [notebook stop:^{
//                       self.finished = YES;
//                   }];
//               }];
//    }];
//}

// ODE


@end
