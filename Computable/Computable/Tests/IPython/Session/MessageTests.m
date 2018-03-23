//
//  MessageTests.m
//  Computable
//
//  Created by Karl Traunmüller on 13.02.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "ComputableTest.h"
#import "../../../Source/App/Core/IPython/Session/Message.h"

@interface MessageTests : ComputableTest
@end


@implementation MessageTests

- (void)testInitializer
{
    Message *message = [Message messageWithIdentity:@"identity"
                                          sessionId:@"sessionId"
                                               type:MessageTypeKernelInfoRequest
                                         completion:^(Message *request, Message *response) {
                                         }];
    STAssertEqualObjects(message.identity, @"identity", @"unexpected identity");
    STAssertEqualObjects(message.sessionId, @"sessionId", @"unexpected sessionId");
    STAssertEquals(message.type, MessageTypeKernelInfoRequest, @"unexpected message type");
    STAssertNotNil(message.msgId, @"msgId is nil");
    STAssertNotNil(message.completion, @"completion is nil");
    STAssertNil(message.parentMsgId, @"didn\"t expect parentMsgId");
    STAssertNil(message.date, @"didn\"t expect date");
    
    STAssertNotNil(message.content, @"expected content");
    STAssertEquals(message.status, StatusUnkown, @"unexpected status");
    STAssertTrue(message.executionCount == NSNotFound, @"unexpected executionCount");
    
    STAssertFalse(message.isStream, @"didn\"t expect stream message");
    STAssertEquals(message.streamType, StreamTypeUnknown, @"unexpected streamType");
    STAssertNil(message.streamData, @"didn\"t expect streamData");
    
    STAssertFalse(message.isInput, @"didn\"t expect an input message");
    STAssertNil(message.input, @"didn\"t expect input");

    STAssertFalse(message.isError, @"didn\"t expect error message");
    STAssertNil(message.errorName, @"didn\"t expect error name");
    STAssertNil(message.errorValue, @"didn\"t expect error value");
    STAssertNil(message.errorTraceback, @"didn\"t expect error traceback");
    
    STAssertNotNil(message.parts, @"expected parts");
}

- (void)testMessageFromParts
{
    NSArray *parts = @[ @"<IDS|MSG>",
                        @"", // signature
                        @"{\"date\": \"2014-02-13T20:15:10.000000\", \"username\": \"kernel\", \"session\": \"c59b5585-312f-4ab2-bd42-15ea61cd88a7\", \"msg_id\": \"f02bf709-1375-4274-b2bd-7be14cec1e06\", \"msg_type\": \"execute_reply\"}", // header
                        @"{\"username\": \"computable\", \"session\": \"c59b5585-312f-4ab2-bd42-15ea61cd88a7\", \"msg_id\": \"37bd2e7c-466f-43ff-a4f0-102ee6fbf041\", \"msg_type\": \"execute_request\"}", // parentHeader
                        @"{\"dependencies_met\": \"True\", \"engine\": \"b24cbc98-b1dd-43cf-bbc1-28ec078dca5b\", \"status\": \"ok\", \"started\": \"2014-02-13T20:15:09.000000\"}", // metadata
                        @"{\"status\": \"ok\", \"execution_count\": 1, \"user_variables\": {}, \"payload\": [], \"user_expressions\": {}}", // content
                       ];
    Message *message = [Message messageFromParts:parts];
    
    STAssertNil(message.identity, @"unexpected identity");
    STAssertEqualObjects(message.sessionId, @"c59b5585-312f-4ab2-bd42-15ea61cd88a7", @"unexpected session id");
    STAssertEqualObjects(message.username, @"kernel", @"unexpected username");
    STAssertTrue(message.type == MessageTypeExecuteReply, @"expected type execute reply");
    STAssertEqualObjects(message.msgId, @"f02bf709-1375-4274-b2bd-7be14cec1e06", @"unexpected message id");

    STAssertEqualObjects(message.parentMsgId, @"37bd2e7c-466f-43ff-a4f0-102ee6fbf041", @"unexpected parent message id");
    STAssertNotNil(message.date, @"expected a date");
    STAssertNotNil(message.content, @"expected non-nil content");

    STAssertTrue(message.status == StatusOk, @"unexpected status");
    STAssertTrue(message.executionCount == 1, @"unexpected execution count");

    STAssertFalse(message.isStream, @"didn\"t expect stream message");
    STAssertTrue(message.streamType == StreamTypeUnknown, @"didn\"t expect a stream type");
    STAssertNil(message.streamData, @"didn\"t expect stream data");

    STAssertFalse(message.isInput, @"didn\"t expect input message");
    STAssertNil(message.input, @"unexpected input");

    STAssertFalse(message.isError, @"didn\"t expect error message");
    STAssertNil(message.errorName, @"didn\"t expect error name");
    STAssertNil(message.errorValue, @"didn\"t expect error value");
    STAssertNil(message.errorTraceback, @"didn\"t expect error traceback");
}

@end
