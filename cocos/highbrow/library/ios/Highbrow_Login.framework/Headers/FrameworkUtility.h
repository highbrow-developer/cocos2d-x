//
//  FrameworkUtility.h
//  Highbrow_Framework
//
//  Created by 김수영 on 14. 6. 2..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "AFNetworking.h"


#include "FrameworkString.h"
#include "LoginViewController.h"
#include "PopupViewController.h"
#include "ProvisionViewController.h"
#include "RegisterViewController.h"
#include "LoadingViewController.h"

//Google
#import "/Highbrow_SubFramework/GooglePlayGames.framework/Headers/GooglePlayGames.h"
#import "/Highbrow_SubFramework/GooglePlus.framework/Headers/GooglePlus.h"
#import "/Highbrow_SubFramework/GoogleOpenSource.framework/Headers/GoogleOpenSource.h"
//Facebook
#import "/Highbrow_SubFramework/FacebookSDK.framework/Headers/FacebookSDK.h"
//Kakao
#import "/Highbrow_SubFramework/KakaoOpenSDK.framework/Headers/KakaoOpenSDK.h"



#define MoveTime        0.3f
#define LoadingView     0
#define PopupView       1
#define LoginView       2
#define ProvisionView   3
#define RegisterView    4

typedef enum {
    facebook,
    googlePlus,
    kakao
}TargetSns;


@interface FrameworkUtility : NSObject <GPPSignInDelegate, UIApplicationDelegate>{
    
    
    /*Options*/
    bool isTestUrl;
    bool isGuestFunction;
    bool isGoogle;
    bool isFacebook;
    NSString *strGameUrl;

    
    /*About View & Screen Size*/
    bool isLoginViewOn;
    CGRect screenRect;
    UIView *parentView;
    
    bool isRequestSnsStatus;
    
    /*Loading View*/
    LoadingViewController *loadingView;
    
    /*About Login*/
    LoginViewController *loginView;
    PopupViewController *popupView;
    ProvisionViewController *provisionView;
    RegisterViewController *registerView;
    
    /*guest response*/
    int ti;
    NSString *token;
    int userNo;
    
    /*register response && login params*/
    int an;
    NSString *sig;
    NSString *email;
    NSString *userLoginId;
    NSString *userRegisterPw;
    
    /*Game Infos*/
    NSString *gameName;
    NSString *langeageCode;
    
    /*Sns Id*/
    NSString *userSnsId;//<< result id
    NSString *userSnsType;
    NSString *userSnsFriend;
    
    /*
    NSString *userFacebookId;
    NSString *userFacebookFriendInfo;
    NSString *userGoogleId;
    */
    
    /*delegate*/
    id loginFinishTarget;
    SEL loginFinishSelector;
    id registerFinishTarget;
    SEL registerFinishSelector;
    id guestFinishTarget;
    SEL guestFinishSelector;
    
    id facebookFinishTarget;
    SEL facebookFinishSelector;
    id googleFinishTarget;
    SEL googleFinishSelector;
    
    id viewFinishTarget;
    SEL viewFinishSelector;
    
    
    /*google client id*/
    NSString *kClientID;
    
    
    
    /*ETC*/
    int googlePlayAuthCheckCnt;
    TargetSns targetSns;
    
    
    bool startWithRegisterView;
    
    
}

@property(strong) NSDictionary *HttpData;



/************  developer Func  *************/
//Set Game Infos
-(void)setGameInfos:(NSString*)gameName :(NSString*)langeageCode :(NSString*)googleClientId;

/*Set Options*/
-(void)setOptionTest:(bool)isTest;
-(void)setOptionGuest:(bool)isGuestFunction;
-(void)setOptionGoogle:(bool)isGoogle;
-(void)setOptionFacebook:(bool)isFacebook;
-(void)setOptionGameUrl:(NSString*)gameUrl;


-(void)setGuestFinishDelegate:(id)aTarget selector:(SEL)aSelector;
-(void)setRegisterFinishDelegate:(id)aTarget selector:(SEL)aSelector;
-(void)setLoginFinishDelegate:(id)aTarget selector:(SEL)aSelector;
-(void)setFacebookFinishDelegate:(id)aTarget selector:(SEL)aSelector;
-(void)setGoogleFinishDelegate:(id)aTarget selector:(SEL)aSelector;
-(void)setViewFinishDelegate:(id)aTarget selector:(SEL)aSelector;
-(void)setHighbrowLogout;
-(void)setHighbrowLoginFinish;

-(void)setHighbrowForeGround;
-(void)setHighbrowBackGround;


/*User Infos */
-(int) GetUserTi;
-(int) GetUserNo;
-(int) GetUserAccountNo;
-(NSString*) GetUserSig;
-(NSString*) GetUserEmail;
-(NSString*) GetUserToken;

-(NSString*) GetUserLoginId;

-(NSString*) GetUserSnsId;
-(NSString*) GetUserSnsType;
-(NSString*) GetUserSnsFriend;

//-(NSString*) GetUserFacebookId;
//-(NSString*) GetUserFacebookFriendInfo;
//-(NSString*) GetUserGoogleId;


/************  inner Func  *************/
+(id)sharedManager;

-(CGRect) getScreenRect;
-(UIView*) getParentView;
-(void)initValues;
-(NSString*) getGameId;


/*Request Funcs*/
-(void)gameServer_Guest:(UIView*)parentView;
-(void)loginServer_Register:(UIView*)parentView :(NSString*)userId :(NSString*)userPw :(NSString*)userEmail;
-(void)loginServer_Login:(UIView*)parentView :(NSString*)userId :(NSString*)userPw;

-(void)loginServer_LoginSns;
- (NSString *) md5:(NSString *) input;

-(void)initLayer:(int)kind;
-(ProvisionViewController*) getProvisionView;
-(LoginViewController*) getLoginView;
-(PopupViewController*) getPopupView;
-(RegisterViewController*) getRegisterView;
-(LoadingViewController*) getLoadingView;

-(void) LoadingShow;
-(void) LoadingDestroy:(BOOL)isForce;
-(void) LoadingDestroy;

-(void) LoginViewShow:(UIView*)parentView;
-(void) PopupViewShow:(NSString*)str;
-(void) PopupViewShow:(NSString*)str:(BOOL)isFindId;
-(void) ProvisionViewShow;
-(void) RegisterViewShow:(UIView*)parentView;

-(BOOL) NSStringIsValidEmail:(NSString *)checkString;
-(void) RequestSite:(NSString*)url :(NSString*)param;
-(void) RequestSite:(NSString*)url;

-(void) ViewScaleEffect:(UIView*)targetView;
-(bool) GetStartWithRegister;
-(void) setInitStartWithRegister;
-(bool) getOptionGuest;
-(bool) getOptionGoogle;
-(bool) getOptionFacebook;
-(NSString*)getOptionGameUrl;

-(void)setViewFinish;

/*SNS Login*/
-(void)requestFacebookLogin;
-(void)requestGoogleLogin;
- (BOOL)openURLListener:(NSURL *)url :(NSString *)sourceApplication :(id)annotation;



@end
