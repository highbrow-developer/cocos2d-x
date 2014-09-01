//
//  HighbrowLogin.h
//  Highbrow_Framework
//
//  Created by 김수영 on 14. 6. 2..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol HighbrowLoginDelegate <NSObject>

@required
- (void)highbrowLoginDidFinishLogin:(NSDictionary *)data;

@end

@interface HighbrowLogin : NSObject

+ (void)setDelegate:(id<HighbrowLoginDelegate>)delegate gameId:(NSString *)gameId language:(NSString *)languageCode testMode:(BOOL)isTest;
+ (void)showLoginView;
+ (void)hideLoginView;

@end
