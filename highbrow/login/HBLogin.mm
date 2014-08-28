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

@interface HBLogin_objc : NSObject <HighbrowLoginDelegate>

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
}

@end

#pragma mark - HBLogin

void HBLogin::init()
{
    [[HBLogin_objc sharedInstance] initData];
}

void HBLogin::showLoginPopup()
{
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
