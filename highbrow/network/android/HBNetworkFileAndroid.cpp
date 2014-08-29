//
//  HBNetworkFileDownload.h
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 25..
//
//

#include "HBNetworkFileAndroid.h"

NS_CC_BEGIN

HBNetworkFileAndroid::HBNetworkFileAndroid()
{
    
}

HBNetworkFileAndroid::~HBNetworkFileAndroid()
{

}

HBNetworkFile* HBNetworkFile::getInstance()
{
    if ( s_pHBNetworkFile == nullptr )
    {
        s_pHBNetworkFile = new HBNetworkFileAndroid();
        s_pHBNetworkFile->init();
    }
    
    return s_pHBNetworkFile;
}

void HBNetworkFileAndroid::downloadTask(std::string strUrl, std::string strPath)
{

}

long long HBNetworkFileAndroid::getMemorySize()
{
    long long nResult = 0;
    
    
    
    return nResult;
}

NS_CC_END

