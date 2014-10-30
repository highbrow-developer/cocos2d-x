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

- (void)initData:(NSString *)gameId language:(NSString *)languageCode
{
    [HighbrowLogin setDelegate:[HBLogin_objc sharedInstance] gameId:gameId language:languageCode testMode:YES];
}

- (void)highbrowLoginDidFinishLogin:(NSDictionary *)data
{
    NSLog(@"%@", data);
    
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:data options:NULL error:&error];

    if (! jsonData) {
        NSLog(@"JSON Write Error: %@", error.localizedDescription);
    } else {
        const char* jsonText = (const char*)[jsonData bytes];
        _callback(jsonText);
    }
}

@end

#pragma mark - HBLogin

void HBLogin::init(const char *gameId, const char *languageCode)
{
    [[HBLogin_objc sharedInstance] initData:[NSString stringWithUTF8String:gameId] language:[NSString stringWithUTF8String:languageCode]];
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
