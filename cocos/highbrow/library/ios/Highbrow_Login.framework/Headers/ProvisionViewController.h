//
//  ProvisionViewController.h
//  Highbrow_Framework
//
//  Created by 김수영 on 14. 6. 3..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ProvisionViewController : UIViewController{
    
    
    IBOutlet UIImageView *imageView_Bg1;
    IBOutlet UIImageView *imageView_Top1;
    IBOutlet UIImageView *imageView_Bg2;
    IBOutlet UIImageView *imageView_Top2;
    IBOutlet UIButton *button_Close;
    IBOutlet UILabel *label_ProTitle1;
    IBOutlet UILabel *label_ProTitle2;
    IBOutlet UIImageView *imageView_TextBg1;
    IBOutlet UIImageView *imageView_TextBg2;
    IBOutlet UIButton *button_Detail1;
    IBOutlet UIButton *button_Detail2;
    IBOutlet UILabel *label_Argee1;
    IBOutlet UILabel *label_Argee2;
    IBOutlet UIImageView *imageView_CheckBg1;
    IBOutlet UIImageView *imageView_CheckBg2;
    IBOutlet UILabel *label_ProText1;
    IBOutlet UILabel *label_ProText2;
    IBOutlet UIButton *button_Check1;
    IBOutlet UIButton *button_Check2;
    
    
    
    bool isCheck1;
    bool isCheck2;
    bool isEnableButton;
    
}

@property(nonatomic, retain)IBOutlet UIImageView *imageView_Bg1;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Top1;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Bg2;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Top2;

@property(nonatomic, retain)IBOutlet UIButton *button_Close;

@property(nonatomic, retain)IBOutlet UILabel *label_ProTitle1;
@property(nonatomic, retain)IBOutlet UILabel *label_ProTitle2;

@property(nonatomic, retain)IBOutlet UIImageView *imageView_TextBg1;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_TextBg2;

@property(nonatomic, retain)IBOutlet UIButton *button_Detail1;
@property(nonatomic, retain)IBOutlet UIButton *button_Detail2;

@property(nonatomic, retain)IBOutlet UILabel *label_Argee1;
@property(nonatomic, retain)IBOutlet UILabel *label_Argee2;

@property(nonatomic, retain)IBOutlet UIImageView *imageView_CheckBg1;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_CheckBg2;

@property(nonatomic, retain)IBOutlet UILabel *label_ProText1;
@property(nonatomic, retain)IBOutlet UILabel *label_ProText2;

@property(nonatomic, retain)IBOutlet UIButton *button_Check1;
@property(nonatomic, retain)IBOutlet UIButton *button_Check2;


-(IBAction)click_Close:(id)sender;
-(IBAction)click_Detail1:(id)sender;
-(IBAction)click_Detail2:(id)sender;
-(IBAction)click_Check1:(id)sender;
-(IBAction)click_Check2:(id)sender;

-(void)loadViewRegister;
-(void)destroy;


@end
