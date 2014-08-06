//
//  HBnetwork.h
//  cocos2d_libs
//
//  Created by 김수영 on 2014. 8. 6..
//
//

#ifndef __cocos2d_libs__HBnetwork__
#define __cocos2d_libs__HBnetwork__

#include "cocos2d.h"

class CC_DLL HBnetwork
{
    public :
    HBnetwork();
    ~HBnetwork();
    
    static HBnetwork* sharedSystem();
    
    //함수
    public :
    
    void requestTest();
    
//    int aaa;
    
};


#endif /* defined(__cocos2d_libs__HBnetwork__) */
