//
//  HBNetworkFileDownload.h
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 25..
//
//

#ifndef cocos2d_libs_HBNetworkFileAndroid_h
#define cocos2d_libs_HBNetworkFileAndroid_h

#include "HBNetworkFile.h"

NS_CC_BEGIN

class CC_DLL HBNetworkFileAndroid : public HBNetworkFile
{
public:
    
    HBNetworkFileAndroid(void);
    virtual ~HBNetworkFileAndroid(void);

public:
    /** 메모리 용량 */
    virtual long long getMemorySize() override;
    
    /** 다운로드 */
    virtual void downloadTask(std::string strUrl, std::string strPath) override;
    
    
private:
protected:


};

NS_CC_END

#endif
