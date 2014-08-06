//
//  PopupViewController.h
//  Highbrow_Framework
//
//  Created by 김수영 on 14. 6. 3..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PopupViewController : UIViewController{
    
    
    IBOutlet UIImageView *imageView_Highbrow;
    IBOutlet UIImageView *imageView_Bg;
    IBOutlet UIImageView *imageView_Top;
    IBOutlet UILabel *label_Content;
    IBOutlet UIButton *button_Confirm;
    IBOutlet UIButton *button_Close;
    
    id target;
    SEL selector;
    
    
    
    
}

- (void)CallBackMethod:(id)result_obj;
- (void)setDelegate:(id)aTarget selector:(SEL)aSelector;


@property(nonatomic, retain)IBOutlet UIImageView *imageView_Highbrow;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Bg;
@property(nonatomic, retain)IBOutlet UIImageView *imageView_Top;

@property(nonatomic, retain)IBOutlet UILabel *label_Content;

@property(nonatomic, retain)IBOutlet UIButton *button_Close;
@property(nonatomic, retain)IBOutlet UIButton *button_Confirm;


-(IBAction)click_Close:(id)sender;
-(IBAction)click_Confirm:(id)sender;

-(void) setContentText:(NSString*)str;
-(void) setConfirmText:(NSString*)str;
-(void) destroy;


@end
