//
//  HBnetwork.cpp
//  cocos2d_libs
//
//  Created by 김수영 on 2014. 8. 6..
//
//

#include "HBnetwork.h"


static HBnetwork *s_pSystem = NULL;
HBnetwork::HBnetwork(){
    
}
HBnetwork::~HBnetwork()
{
    if (s_pSystem)
    {
        delete s_pSystem;
        s_pSystem = NULL;
    }
}

HBnetwork* HBnetwork::sharedSystem()
{
    if ( s_pSystem == NULL )
    {
        s_pSystem = new HBnetwork();
    }
    
    return s_pSystem;
}


HBnetwork* HBnetwork::requestTest(){
    
    log("requestTest");
}
