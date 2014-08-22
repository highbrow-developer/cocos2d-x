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
   
    //error
    HBJson *pError = HBJson::create();
    pError->addData(false);
    if ( data == nullptr || document.HasParseError() == true )
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
