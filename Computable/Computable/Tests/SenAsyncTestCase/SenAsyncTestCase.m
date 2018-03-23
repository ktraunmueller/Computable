#import "SenAsyncTestCase.h"

static NSTimeInterval const SenDefaultTimeoutInterval = 10.f;

@implementation SenAsyncTestCase

#pragma mark - setUp / tearDown

- (void)setUp
{
    [super setUp];
    
    self.timeoutInterval = SenDefaultTimeoutInterval;
    self.finished = NO;
}

- (void)tearDown
{
    double delayInSeconds = self.timeoutInterval;
    dispatch_time_t popTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(delayInSeconds * NSEC_PER_SEC));
    dispatch_after(popTime, dispatch_get_main_queue(), ^(void){
        STFail(@"timed out.");
        self.finished = YES;
    });
    
    do {
        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:.1]];
    } while (!self.isFinished);
    
    [super tearDown];
}

@end
