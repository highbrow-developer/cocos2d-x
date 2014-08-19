
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
//#include "highbrow/libs/librapidjson/document.h"
#include "external/json/document.h"


#define PARSE_HBDATA(data)                          HBNetwork::parseDocument(data)


NS_CC_BEGIN

using namespace rapidjson;

#define NO_DATA_NUMBER                              0.9931f
#define NO_DATA_STRING                              ""

/*  데이터 수신 포멧  */
typedef struct {
    bool _bool;
    float _number;
    std::string _string;
} _data_struct;

class HBData : public Ref {
public:
    HBData();
    ~HBData();
    
    static HBData* create();
    
protected:
    
public:
    const int size();
    
    bool isBool(int index = 0);
    bool isInt(int index = 0);
    bool isFloat(int index = 0);
    bool isString(int index = 0);
    bool isArray();
    bool isObject();
    
    void addData(bool data);
    void addData(int data);
    void addData(float data);
    void addData(double data);
    void addData(const char *data);
    void addData(HBData *data);
    
    bool getBool(int index = 0);
    int getInt(int index = 0);
    float getFloat(int index = 0);
    const char* getString(int index = 0);
    HBData* getArray(int index = 0);
    
    
    HBData* at(const char *key);
    void insert(const char *key, HBData *data);
    
protected:
    bool initData();
    
public:
protected:
    int _index;
    
    std::vector<_data_struct> _data;
    std::vector<HBData*> _data_array;
    
    Map<std::string, HBData*> _map;
};


class HBNetwork : public Ref {
public:
    /*  맵 변환  */
    static Map<std::string, HBData*> parseDocument(std::vector<char> *data) {//Document document) { //
        Map<std::string, HBData*> map;
        
//        data->push_back('\0');
//        char *json = reinterpret_cast<char*>(data->data());
        
        /*  로그  */
//        log("response json : %s", json);
        
        rapidjson::Document document;
        document.Parse<0>("{\"netflag\":0,\"_ti\":1407751270,\"dragons\":{\"16036\":[0,2,3,4,5,7]},\"battle\":{\"log\":[{\"dn\":1,\"sq\":[{\"an\":1,\"tn\":{\"4\":4},\"sn\":[1],\"hp\":{\"4\":-33},\"mission_24\":1}]},{\"dn\":4,\"sq\":[{\"an\":4,\"tn\":{\"1\":1},\"sn\":[1],\"hp\":{\"1\":-6}}]},{\"dn\":1,\"sq\":[{\"an\":1,\"tn\":{\"4\":4},\"sn\":[1],\"hp\":{\"4\":-35},\"mission_24\":2,\"dead\":[4]}]}],\"win\":\"a\",\"mission\":[],\"position\":{\"1\":16036,\"4\":1}},\"mission_battle\":[[24,\"4\ubc88 \uc774\uc0c1 \ub54c\ub9ac\uae30\",4,20],[4,\"3\uc5f0\ud0c0 \ub54c\ub9ac\uae30\",1,40]],\"change1\":[{\"tag\":16036,\"exp_max\":36,\"exp_now\":22,\"exp_prev\":9}],\"hp\":{\"hp_now1\":196,\"hp_max1\":202,\"hp_now2\":0,\"hp_max2\":0,\"hp_now3\":0,\"hp_max3\":0},\"quest_check\":1,\"rs\":0,\"etc\":{\"exp1\":5,\"exp_bonus1\":0},\"reward\":{\"gold\":18,\"gold_bonus\":0}}");//json);//
        
        if ( document.HasParseError() ) {
            HBData *value = HBData::create();
            value->addData(-1);
            map.insert("rs", value);
            return map;
        }
        
        std::function<HBData*(rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstValueIterator iterator)> parse = [&parse](rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstValueIterator iterator) mutable ->HBData* {
            
            HBData *data_result = HBData::create();
            
                switch ( iterator->GetType() ) {
                    case kFalseType:
                    case kTrueType:     data_result->addData(iterator->GetBool());       break;
                    case kStringType:   data_result->addData(iterator->GetString());     break;
                    case kNumberType:   data_result->addData(iterator->GetDouble());     break;
                    case kObjectType: {
                        for ( rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstMemberIterator itr = iterator->MemberonBegin(); itr != iterator->MemberonEnd(); ++itr ) {
                            data_result->insert(itr->name.GetString(), parse(&itr->value));
                        }
                    }
                        break;
                    case kNullType:
                        data_result->release();
                        data_result = nullptr;
                        break;
                    case kArrayType: {
                        
                        HBData *data_array = HBData::create();
                        
                        std::function<HBData*(rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstValueIterator itr_arr, HBData *data_arr)> parse_arr = [&parse, &parse_arr](rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstValueIterator itr_arr, HBData *data_arr) mutable ->HBData* {
                            
                            for ( rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstValueIterator itr_value = itr_arr->onBegin(); itr_value != itr_arr->onEnd(); ++itr_value ) {
                                switch ( itr_value->GetType() ) {
                                    case kFalseType:
                                    case kTrueType:     data_arr->addData(itr_value->GetBool());       break;
                                    case kStringType:   data_arr->addData(itr_value->GetString());     break;
                                    case kNumberType:   data_arr->addData(itr_value->GetDouble());     break;
                                    case kObjectType: {
                                        for ( rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstMemberIterator itr = itr_value->MemberonBegin(); itr != itr_value->MemberonEnd(); ++itr ) {
                                            data_arr->insert(itr->name.GetString(), parse(&itr->value));
                                        }
                                    }
                                        break;
                                    case kNullType:
                                        data_arr->release();
                                        data_arr = nullptr;
                                        break;
                                    case kArrayType: {
                                        HBData *data_array_inner = HBData::create();
                                        data_arr->addData(parse_arr(itr_value, data_array_inner));
                                    }
                                        break;
                                }
                            }
                            return data_arr;
                        };
                        
                        data_result->addData(parse_arr(iterator, data_array));
                    }
                        break;
                }
            
            return data_result;
        };
        
        for ( rapidjson::GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator> >::ConstMemberIterator itr = document.MemberonBegin(); itr != document.MemberonEnd(); ++itr ) {
            log("%s 파싱 중", itr->name.GetString());
            map.insert(itr->name.GetString(), parse(&itr->value));
        }
        
        return map;
    };
};

NS_CC_END

#endif /* defined(__cocos2d_libs__HBNetwork__) */
