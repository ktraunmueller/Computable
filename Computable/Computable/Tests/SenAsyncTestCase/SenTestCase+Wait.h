#import <SenTestingKit/SenTestingKit.h>

@interface SenTestCase (Wait)

@property (nonatomic, getter = isWaiting) BOOL waiting;

- (void)startWaiting;
- (void)stopWaiting;

@end
