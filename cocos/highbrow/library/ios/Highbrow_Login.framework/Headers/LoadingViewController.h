//
//  LoadingViewController.h
//  Highbrow_Framework
//
//  Created by 김수영 on 14. 6. 9..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface LoadingViewController : UIViewController{
    
//    IBOutlet UIImageView *imageView_Loading;
    
}

@property(nonatomic, retain)IBOutlet UIImageView *imageView_Loading;


-(void)destroy:(BOOL)isForce;

@end



