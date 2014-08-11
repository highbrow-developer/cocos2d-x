//
//  HBNetwork.cpp
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 11..
//
//

#include "HBNetwork.h"

USING_NS_CC;

HBData::HBData() :
_data_int(0),
_data_float(0.0f),
_data_string("") {
    
}

HBData::~HBData() {
    
}

HBData* HBData::create(int data) {
    HBData *ret = new HBData();
    if ( ret && ret->initData(data) ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

HBData* HBData::create(float data) {
    HBData *ret = new HBData();
    if ( ret && ret->initData(data) ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

HBData* HBData::create(std::string data) {
    HBData *ret = new HBData();
    if ( ret && ret->initData(data) ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool HBData::initData(int data) {
    _data_int = data;
    return true;
}

bool HBData::initData(float data) {
    _data_float = data;
    return true;
}

bool HBData::initData(std::string data) {
    _data_string = data;
    return true;
}

int HBData::getInt() {
    return _data_int;
}

float HBData::getFloat() {
    return _data_float;
}

std::string HBData::getString() {
    return _data_string;
}