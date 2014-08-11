//
//  HBMacros.cpp
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 7..
//
//

#include "HBMacros.h"
USING_NS_CC;

HBdata::HBdata() {
    _data_int = 0;
    _data_float = 0.0f;
    _data_string = "";
}

HBdata::~HBdata() {
    
}

HBdata* HBdata::create(void *data) {
    HBdata *ret = new HBdata();
    if ( ret && ret->initData(data) ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool HBdata::initData(void *data) {
    _data_int = (int)data;
    _data_float = (float)data;
    _data_string = dynamic_cast<std::string>data;
}

int HBdata::getInt() {
    return _data_int;
}

float HBdata::getFloat() {
    return _data_float;
}

std::string HBdata::getString() {
    return _data_string;
}