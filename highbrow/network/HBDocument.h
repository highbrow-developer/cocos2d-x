//
//  HBDocument.h
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 8..
//
//

#ifndef __cocos2d_libs__HBDocument__
#define __cocos2d_libs__HBDocument__

#include "cocos2d.h"

#include "document.h"
#include "CCMap.h"

NS_CC_BEGIN

using namespace rapidjson;

class HBDocument : public Ref {
public:
    static Map parseDocument(std::vector<char> *buff) {
        buff->push_back('\0');
        char *json = reinterpret_cast<char*>(buff->data());
        
        /*  로그  */
        log("response json ; %s", json);
        
        Document parser.parse<0>(json);
    };
//    HBDocument();
//    ~HBDocument();
//    
//    static HBDocument* create(std::vector<char> *buff);
//    
//protected:
//    virtual bool initData(std::vector<char> *buff);
//    
//public:
//    int 
//    
//protected:
//    
//public:
//protected:
//    Document _document;
    
};


NS_CC_END

#endif /* defined(__cocos2d_libs__HBDocument__) */
