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
_index(-1)
{
    
}

HBData::~HBData() {
    _data.clear();
    _data_array.clear();
    _map.clear();
}

HBData* HBData::create() {
    HBData *ret = new HBData();
    if ( ret && ret->initData() ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

bool HBData::initData() {
    
    return true;
}

const int HBData::size() {
    return _data.size() + _data_array.size() + _map.size();
}

bool HBData::isBool(int index) {
    if ( _data[index]._bool == false )  return false;
    else                                return true;
}

bool HBData::isInt(int index) {
    if ( _data[index]._number == NO_DATA_NUMBER )   return false;
    else                                            return true;
}

bool HBData::isFloat(int index) {
    if ( _data[index]._number == NO_DATA_NUMBER )   return false;
    else                                            return true;
}

bool HBData::isString(int index) {
    if ( _data[index]._string.length() == 0 )       return false;
    else                                            return true;
}

bool HBData::isArray() {
    if ( _data_array.size() > 1 ) {
        return true;
    }
    
    return false;
}

bool HBData::isObject() {
    if ( _map.size() > 1 ) {
        return true;
    }
    
    return false;
}

void HBData::addData(bool data) {
    _data_struct _struct;
    _struct._bool = data;
    _struct._number = NO_DATA_NUMBER;
    _struct._string = NO_DATA_STRING;
    _data.push_back(_struct);
}

void HBData::addData(int data) {
    _data_struct _struct;
    _struct._bool = false;
    _struct._number = data;
    _struct._string = NO_DATA_STRING;
    _data.push_back(_struct);
}

void HBData::addData(float data) {
    _data_struct _struct;
    _struct._bool = false;
    _struct._number = data;
    _struct._string = NO_DATA_STRING;
    _data.push_back(_struct);
}

void HBData::addData(double data) {
    _data_struct _struct;
    _struct._bool = false;
    _struct._number = data;
    _struct._string = NO_DATA_STRING;
    _data.push_back(_struct);
}

void HBData::addData(const char *data) {
    _data_struct _struct;
    _struct._bool = false;
    _struct._number = NO_DATA_NUMBER;
    _struct._string = data;
    _data.push_back(_struct);
}

void HBData::addData(HBData* data) {
    _data_array.push_back(data);
}


bool HBData::getBool(int index) {
    return _data[index]._bool;
}

int HBData::getInt(int index) {
    return (int)_data[index]._number;
}

float HBData::getFloat(int index) {
    return (float)_data[index]._number;
}

const char* HBData::getString(int index) {
    return _data[index]._string.c_str();
}

HBData* HBData::getArray(int index) {
    return _data_array[index];
}

HBData* HBData::at(const char *key) {
    return _map.at(key);
}

void HBData::insert(const char *key, HBData *data) {
    _map.insert(key, data);
}