//
//  RegisterViewController.h
//  Highbrow_Framework
//
//  Created by 김수영 on 14. 6. 4..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface RegisterViewController : UIViewController{
    
    
    IBOutlet UIImageView *imageView_Bg;
    IBOutlet UIImageView *imageView_Top;
    IBOutlet UITextField *textField_Id;
    IBOutlet UITextField *textField_Email;
    IBOutlet UITextField *textField_Pw1;
    IBOutlet UITextField *textField_Pw2;
    IBOutlet UILabel *label_Title;
    IBOutlet UIButton *button_Close;
    IBOutlet UIButton *button_Confirm;
    IBOutlet UIImageView *imageView_Highbrow;
    
    
    BOOL idPass, emailPass, pw1Pass, pw2Pass;
    NSString *idText, *emailText, *pw1Text, *pw2Text;
    
    
    CGPoint basicPoint;
    int moveHeightId;
    int moveHeightEmail;
    int moveHeightPw1;
    int moveHeightPw2;
}

@property(nonatomic, retain)IBOutlet UIImageView *imageView_Bg;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Top;

@property(nonatomic, retain)IBOutlet UITextField *textField_Id;
@property(nonatomic, retain)IBOutlet UITextField *textField_Email;
@property(nonatomic, retain)IBOutlet UITextField *textField_Pw1;
@property(nonatomic, retain)IBOutlet UITextField *textField_Pw2;

@property(nonatomic, retain)IBOutlet UILabel *label_Title;

@property(nonatomic, retain)IBOutlet UIButton *button_Close;
@property(nonatomic, retain)IBOutlet UIButton *button_Confirm;

@property(nonatomic, retain)IBOutlet UIImageView *imageView_Highbrow;



-(IBAction)touchIn_Id:(id)sender;
-(IBAction)touchIn_Email:(id)sender;
-(IBAction)touchIn_Pw1:(id)sender;
-(IBAction)touchIn_Pw2:(id)sender;

-(IBAction)change_Id:(id)sender;
-(IBAction)change_Email:(id)sender;
-(IBAction)change_Pw1:(id)sender;
-(IBAction)change_Pw2:(id)sender;

-(IBAction)touchOut_Id:(id)sender;
-(IBAction)touchOut_Email:(id)sender;
-(IBAction)touchOut_Pw1:(id)sender;
-(IBAction)touchOut_Pw2:(id)sender;


-(IBAction)click_Close:(id)sender;
-(IBAction)click_Confirm:(id)sender;

-(void)setRegister;
-(void)destroy;

@end
