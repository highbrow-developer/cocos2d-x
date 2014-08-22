//
//  HBNetworkFile.cpp
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 22..
//
//

#include "HBNetworkFile.h"

NS_CC_BEGIN

HBNetworkFile::HBNetworkFile()
{

}

HBNetworkFile::~HBNetworkFile()
{

}

bool HBNetworkFile::init()
{
    return true;
}

HBNetworkFile* HBNetworkFile::create()
{
    auto ret = new HBNetworkFile();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

NS_CC_END
