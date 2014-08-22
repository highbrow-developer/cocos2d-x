//
//  HBNetworkFile.h
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 22..
//
//

#ifndef __cocos2d_libs__HBNetworkFile__
#define __cocos2d_libs__HBNetworkFile__

#include "cocos2d.h"
#include "external/json/document.h"

NS_CC_BEGIN

class CC_DLL HBNetworkFile : public Ref {
public:
    static HBNetworkFile* create();

    
    /** The default constructor. */
    HBNetworkFile(void);
    
    /** The default destructor */
    virtual ~HBNetworkFile(void);
    
    /** Initializes the HBNetworkFile */
    virtual bool init();
    
protected:
    
public:
    
    
private:
   
    
    
protected:

};


NS_CC_END

#endif /* defined(__cocos2d_libs__HBNetworkFile__) */
