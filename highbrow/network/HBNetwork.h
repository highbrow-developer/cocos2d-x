//
//  HBNetwork.h
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 11..
//
//

#ifndef __cocos2d_libs__HBNetwork__
#define __cocos2d_libs__HBNetwork__

#include "cocos2d.h"

/*  json  */
#include "highbrow/libs/librapidjson/document.h"


NS_CC_BEGIN
using namespace rapidjson;


/*  데이터 수신 포멧  */
class HBData : public Ref {
public:
    HBData();
    ~HBData();
    
    static HBData* create(int data);
    static HBData* create(float data);
    static HBData* create(std::string data);
    
protected:
    
public:
    int getInt();
    float getFloat();
    std::string getString();
    
protected:
    bool initData(int data);
    bool initData(float data);
    bool initData(std::string data);
    
public:
protected:
    int _data_int;
    float _data_float;
    std::string _data_string;
};


class HBNetwork : public Ref {
public:
    /*  맵 변환  */
    static Map<std::string, HBData*> parseDocument(std::vector<char> *data) {
        Map<std::string, HBData*> map;
        
        data->push_back('\0');
        char *json = reinterpret_cast<char*>(data->data());
        
        /*  로그  */
        log("response json ; %s", json);
        
        rapidjson::Document document;
        document.Parse<0>(json);
        
        if ( document.HasParseError() ) {
            map.insert("rs", HBData::create(-1));
            return map;
        }
        
//        for ( ConstMemberIterator itr = document.MemberBegin(); itr != document.MenberEnd(); ++itr ) {
//            
//        }
        
        
        //        switch ( parser[index.c_str()]["value"]
        
        
//        for ( int cnt = 0; cnt < parser.Size(); cnt++ ) {
//            
//            std::string index = __String::createWithFormat("%du", cnt)->getCString();
//            
//            key = parser[index.c_str()]["key"].GetString();
//            value = __String::createWithFormat("%s", parser[index.c_str()]["value"].GetString());
//            
//            map.insert(key.c_str(), value);
//        }
//        
        return map;
    };
};

NS_CC_END

#endif /* defined(__cocos2d_libs__HBNetwork__) */
