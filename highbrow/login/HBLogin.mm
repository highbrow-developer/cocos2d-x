//
//  HBLogin.m
//  HBLogin
//
//  Created by Heewon Song on 2014. 8. 21..
//
//

#import "HBLogin.h"
#import <HighbrowLogin/HighbrowLogin.h>

#pragma mark - HBLogin_objc

@interface HBLogin_objc : NSObject <HighbrowLoginDelegate> {
}

@property (nonatomic, readwrite) std::function<void(const char*)> callback;

@end

@implementation HBLogin_objc

+ (HBLogin_objc *)sharedInstance
{
    static HBLogin_objc *myInstance = nil;
    if(!myInstance) {
        myInstance = [[HBLogin_objc alloc] init];
    }
    return myInstance;
}

- (void)initData
{
    [HighbrowLogin setDelegate:[HBLogin_objc sharedInstance] gameId:@"TOF" language:@"kr" testMode:YES];
}

- (void)highbrowLoginDidFinishLogin:(NSDictionary *)data
{
    NSLog(@"%@", data);
    
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:data options:nil error:&error];

    if (! jsonData) {
        NSLog(@"JSON Write Error: %@", error.localizedDescription);
    } else {
        _callback((const char*)[jsonData bytes]);
    }
}

@end

#pragma mark - HBLogin

void HBLogin::init()
{
    [[HBLogin_objc sharedInstance] initData];
}

void HBLogin::showLoginPopup(const std::function<void(const char *)> &callback)
{
    [HBLogin_objc sharedInstance].callback = callback;
    [HighbrowLogin showLoginView];
}

void HBLogin::hideLoadingLayer()
{
    
}

//void HBLogin::signup() {
//}
//
//void HBLogin::logout() {
//}
