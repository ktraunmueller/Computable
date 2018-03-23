//
//  AnsiParser.m
//  Computable
//
//  Created by Karl Traunmüller on 08.05.14.
//  Copyright (c) 2014 microLARGE. All rights reserved.
//

#import "AnsiParser.h"
#import "../../../Common/Utilities/UIColor+AnsiColors.h"
#import "../../../Common/Utilities/strutils.h"

#import <vtparse.h>

@interface ParseContext : NSObject

@property (nonatomic, strong) AnsiParser *owner;
@property (nonatomic, assign) char *msg;
@property (nonatomic, assign) vtparse_t *parser;
@property (nonatomic, strong) NSMutableAttributedString *ansiResponse;
@property (nonatomic, strong) NSMutableDictionary *ansiAttributes;

@end

@implementation ParseContext
@end

@implementation AnsiParser

- (NSAttributedString *)parse:(const char *)ansi
{
//    printf("ansi: %s", ansi);
    
    char escapeSequence[] = { 0x1b, 0 };
    char *escaped = replace_str(ansi, "\\u001b", escapeSequence);
    escaped = replace_str(escaped, "\e", "");
    
    ParseContext *context = [ParseContext new];
    context.owner = self;
    context.msg = escaped;
    context.parser = malloc(sizeof(vtparse_t));
    bzero(context.parser, sizeof(vtparse_t));
    vtparse_init(context.parser, vtparse_callback);
    context.ansiResponse = [[NSMutableAttributedString alloc] init];
    context.ansiAttributes = [NSMutableDictionary dictionary];
    context.parser->user_data = (__bridge void *)context;
    
    [context.ansiResponse beginEditing];
    vtparse(context.parser, (unsigned char *)ansi, strlen(ansi));
    [context.ansiResponse endEditing];
    
    free(context.parser);
    free(escaped);
    
    return context.ansiResponse;
}

- (void)onParseContext:(ParseContext *)context action:(vtparse_action_t)action ch:(unsigned char)ch
{
    struct vtparse *parser = context.parser;
    
    //    printf("Received action %s, state %s\n", ACTION_NAMES[action], STATE_NAMES[parser->state]);
    //    if (ch != 0)
    //    {
    //        printf("Char: 0x%02x ('%c')\n", ch, ch);
    //    }
    
    switch (action) {
        case VTPARSE_ACTION_PRINT:
        {
            if (ch != 0)
            {
                const char utf8[2] = { ch, 0 };
                NSAttributedString *fragment = [[NSAttributedString alloc] initWithString:[NSString stringWithUTF8String:&utf8[0]]
                                                                               attributes:context.ansiAttributes];
                [context.ansiResponse appendAttributedString:fragment];
            }
        }
            break;
        case VTPARSE_ACTION_CSI_DISPATCH:
            switch (parser->state) {
                case VTPARSE_STATE_CSI_PARAM:
                    switch (parser->num_params) {
                        case 1:
                            if (parser->params[0] == 0)
                            {
                                [context.ansiAttributes removeAllObjects]; // reset attributes
                            }
                            break;
                        case 2: {
                            if (parser->params[0] == 0)
                            {
                                [context.ansiAttributes removeAllObjects]; // reset attributes
                            }
                            UIColor *color = [UIColor ansiColor:parser->params[1]];
                            if (color)
                            {
                                [context.ansiAttributes setObject:color forKey:NSForegroundColorAttributeName];
                            }
                        }
                            [context.ansiResponse setAttributes:context.ansiAttributes range:NSMakeRange(context.ansiResponse.length - 1, 0)];
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            break;
        case VTPARSE_ACTION_EXECUTE:
            break;
        case VTPARSE_ACTION_ERROR:
            break;
        default:
            break;
    }
    
    int i = 0;
    if (parser->num_intermediate_chars > 0)
    {
        printf("%d Intermediate chars:\n", parser->num_intermediate_chars);
        for (i = 0; i < parser->num_intermediate_chars; i++)
        {
            printf("  0x%02x ('%c')\n", parser->intermediate_chars[i],
                   parser->intermediate_chars[i]);
        }
    }
    //    if (parser->num_params > 0)
    //    {
    //        printf("%d Parameters:\n", parser->num_params);
    //        for (i = 0; i < parser->num_params; i++)
    //        {
    //            printf("\t%d\n", parser->params[i]);
    //        }
    //    }
}

static void vtparse_callback(struct vtparse *parser, vtparse_action_t action, unsigned char ch)
{
    ParseContext *context = (__bridge ParseContext *)parser->user_data;
    [context.owner onParseContext:context action:action ch:ch];
}

@end
