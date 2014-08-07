//
//  LoginViewController.h
//  HB_iOS_Login_Framework
//
//  Created by 김수영 on 14. 5. 30..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface LoginViewController : UIViewController <NSXMLParserDelegate>{
    
    
    IBOutlet UIImageView *imageView_Highbrow;
    IBOutlet UIImageView *imageView_Bg;
    IBOutlet UIImageView *imageView_Top;
    IBOutlet UITextField *textField_Id;
    IBOutlet UITextField *textField_Pw;
    IBOutlet UIButton *button_ExMark;
    IBOutlet UIButton *button_Close;
    IBOutlet UIButton *button_Facebook;
    IBOutlet UIButton *button_Google;
    IBOutlet UIButton *button_Confirm;
    IBOutlet UIButton *button_Quest;
    IBOutlet UIButton *button_Register;
    
    BOOL idPass, pwPass;
    
    NSString *idText, *pwText;
    
    
    CGPoint basicPoint;
    int moveHeightId;
    int moveHeightPw;
    
}

- (void)guestLogin;
- (void)boardHideListener;
- (void)loginViewReload;

//@property int idEditCount, pwEditCount;


@property(nonatomic, retain)IBOutlet UIImageView *imageView_Highbrow;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Bg;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Top;

@property(nonatomic, retain)IBOutlet UITextField *textField_Id;

@property(nonatomic, retain)IBOutlet UITextField *textField_Pw;

@property(nonatomic, retain)IBOutlet UIButton *button_ExMark;
@property(nonatomic, retain)IBOutlet UIButton *button_Close;

@property(nonatomic, retain)IBOutlet UIButton *button_Facebook;
@property(nonatomic, retain)IBOutlet UIButton *button_Google;
@property(nonatomic, retain)IBOutlet UIButton *button_Confirm;
@property(nonatomic, retain)IBOutlet UIButton *button_Quest;
@property(nonatomic, retain)IBOutlet UIButton *button_Register;


//-(IBAction)touchUp_Id:(id)sender;
//-(IBAction)touchUp_Pw:(id)sender;

-(IBAction)touchIn_Id:(id)sender;
-(IBAction)touchIn_Pw:(id)sender;
-(IBAction)touchOut_Id:(id)sender;
-(IBAction)touchOut_Pw:(id)sender;
-(IBAction)touchChange_Id:(id)sender;
-(IBAction)touchChange_Pw:(id)sender;

-(IBAction)click_ExMark:(id)sender;
-(IBAction)click_Close:(id)sender;

-(IBAction)click_Facebook:(id)sender;
-(IBAction)click_Google:(id)sender;
-(IBAction)click_Confirm:(id)sender;
-(IBAction)click_Quest:(id)sender;
-(IBAction)click_Register:(id)sender;

-(void) destroy;


@property (strong, nonatomic) LoginViewController *viewController;


@end
