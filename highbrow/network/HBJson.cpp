//
//  HBJson.cpp
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 21..
//
//

#include "HBJson.h"

NS_CC_BEGIN
using namespace rapidjson;

HBJson::HBJson() :
_s(""),
_t(kNullType)
{
    //initializes the number
    _n.i64 = 0;
}

HBJson::~HBJson()
{
    _o.clear();
    _a.clear();
}

bool HBJson::init()
{
    return true;
}

HBJson* HBJson::create()
{
    auto ret = new HBJson();
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

#pragma mark - parse
Map<std::string, HBJson*> HBJson::parse(std::vector<char> *data)
{
    rapidjson::Document document;
    Map<std::string, HBJson*> listJson;
    
    if ( data != nullptr )
    {
        //add null
        data->push_back('\0');
        
        //to char
        char *json = reinterpret_cast<char*>(data->data());
        
        //to rapidjson
        document.Parse<0>(json);
    }
    /*임시*/
    document.Parse<0>("{\"netflag\":0,\"_ti\":1407751270,\"dragons\":{\"16036\":[0,2,3,4,5,7]},\"battle\":{\"log\":[{\"dn\":1,\"sq\":[{\"an\":1,\"tn\":{\"4\":4},\"sn\":[1],\"hp\":{\"4\":-33},\"mission_24\":1}]},{\"dn\":4,\"sq\":[{\"an\":4,\"tn\":{\"1\":1},\"sn\":[1],\"hp\":{\"1\":-6}}]},{\"dn\":1,\"sq\":[{\"an\":1,\"tn\":{\"4\":4},\"sn\":[1],\"hp\":{\"4\":-35},\"mission_24\":2,\"dead\":[4]}]}],\"win\":\"a\",\"mission\":[],\"position\":{\"1\":16036,\"4\":1}},\"mission_battle\":[[24,\"4\ubc88 \uc774\uc0c1 \ub54c\ub9ac\uae30\",4,20],[4,\"3\uc5f0\ud0c0 \ub54c\ub9ac\uae30\",1,40]],\"change1\":[{\"tag\":16036,\"exp_max\":36,\"exp_now\":22,\"exp_prev\":9}],\"hp\":{\"hp_now1\":196,\"hp_max1\":202,\"hp_now2\":0,\"hp_max2\":0,\"hp_now3\":0,\"hp_max3\":0},\"quest_check\":1,\"rs\":0,\"etc\":{\"exp1\":5,\"exp_bonus1\":0},\"reward\":{\"gold\":18,\"gold_bonus\":0}}");
    
    //error
    HBJson *pError = HBJson::create();
    pError->addData(false);
    if ( (data == nullptr && false/*임시*/) || document.HasParseError() == true )
    {
        pError->addData(true);
        
        listJson.insert("error", pError);
        return listJson;
    }
    
    for ( GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator> >::ConstMemberIterator itr = document.MemberonBegin(); itr != document.MemberonEnd(); ++itr )
    {
        listJson.insert(itr->name.GetString(), parseValue(&itr->value));
    }
    
    return listJson;
}

HBJson* HBJson::parseValue(GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstValueIterator iterator)
{
    HBJson *data_result = HBJson::create();
    
    switch ( iterator->GetType() )
    {
        case rapidjson::kNullType:
            CC_SAFE_RELEASE_NULL(data_result);
            break;
        case kFalseType:
        case kTrueType:
            data_result->addData(iterator->GetBool());
            break;
            
        case kStringType:
            data_result->addData(iterator->GetString());
            break;
            
        case kNumberType:
            if ( iterator->IsInt() )            data_result->addData(iterator->GetInt());
            else if ( iterator->IsUint() )      data_result->addData(iterator->GetUint());
            else if ( iterator->IsInt64() )     data_result->addData(iterator->GetInt64());
            else if ( iterator->IsUint64() )    data_result->addData(iterator->GetUint64());
            else data_result->addData(iterator->GetDouble());
            break;
            
        case kObjectType:
            for ( GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstMemberIterator itr = iterator->MemberonBegin(); itr != iterator->MemberonEnd(); ++itr )
            {
                data_result->addData(itr->name.GetString(), parseValue(&itr->value));
            }
            break;
            
        case kArrayType:
            for ( GenericDocument<UTF8<char>, MemoryPoolAllocator<CrtAllocator>>::ConstValueIterator itr = iterator->onBegin(); itr != iterator->onEnd(); ++itr )
            {
                data_result->addData(parseValue(itr));
            }
            break;
    }
    
    return data_result;
}

NS_CC_END
