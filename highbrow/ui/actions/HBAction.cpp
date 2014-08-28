//
//  HBAction.cpp
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 22..
//
//

#include "HBAction.h"

USING_NS_CC;

#pragma mark - Shake
HBShake* HBShake::create( float d, float strength ) {
    return create( d, strength, strength );
}

HBShake* HBShake::create(float duration, float strength_x, float strength_y) {
    HBShake *a = new HBShake();
    a->initWithDuration(duration, strength_x, strength_y);
    a->autorelease();
    
    return a;
}

bool HBShake::initWithDuration(float duration, float strength_x, float strength_y) {
    if ( ActionInterval::initWithDuration(duration) ) {
        _strength_x = strength_x;
        _strength_y = strength_y;
        
        return true;
    }
    
    return false;
}

HBShake* HBShake::clone(void) const {
    auto a = new HBShake();
    a->initWithDuration(_duration, _strength_x, _strength_y);
	a->autorelease();
	return a;
}

HBShake* HBShake::reverse() const {
    return HBShake::create(_duration, _strength_x, _strength_y);
}

float fgRangeRand(float min, float max) {
    float rnd = ((float)rand() / (float)RAND_MAX);
    return rnd * (max - min) + min;
}

void HBShake::update(float time) {
    float randx = fgRangeRand( -_strength_x, _strength_x );
    float randy = fgRangeRand( -_strength_y, _strength_y );
    
    _target->setPosition(Point(_initial_x + randx, _initial_y + randy));
}

void HBShake::stop() {
    _target->setPosition(Point(_initial_x, _initial_y));
    
    ActionInterval::stop();
}

void HBShake::startWithTarget(Node *target) {
    ActionInterval::startWithTarget(target);
    _initial_x = target->getPosition().x;
    _initial_y = target->getPosition().y;
}


#pragma mark - SizeTo
HBSizeTo* HBSizeTo::create(float duration, float s) {
    HBSizeTo *a = new HBSizeTo();
    a->initWithDuration(duration, s);
    a->autorelease();
    
    return a;
}

HBSizeTo* HBSizeTo::create(float duration, float sx, float sy) {
    HBSizeTo *pSizeTo = new HBSizeTo();
    pSizeTo->initWithDuration(duration, sx, sy);
    pSizeTo->autorelease();
    
    return pSizeTo;
}

bool HBSizeTo::initWithDuration(float duration, float s) {
    if ( ActionInterval::initWithDuration(duration) ) {
        _endSizeX = s;
        _endSizeY = s;
        
        return true;
    }
    
    return false;
}

bool HBSizeTo::initWithDuration(float duration, float sx, float sy) {
    if ( ActionInterval::initWithDuration(duration) ) {
        _endSizeX = sx;
        _endSizeY = sy;
        
        return true;
    }
    
    return false;
}

HBSizeTo* HBSizeTo::clone(void) const {
	auto a = new HBSizeTo();
	a->initWithDuration(_duration, _endSizeX, _endSizeY);
	a->autorelease();
	return a;
}

HBSizeTo* HBSizeTo::reverse() const {
	CCAssert(false, "reverse() not supported in ScaleTo");
	return nullptr;
}

void HBSizeTo::startWithTarget(Node *target) {
    ActionInterval::startWithTarget(target);
    _startSizeX = target->getContentSize().width;
    _startSizeY = target->getContentSize().height;
    _deltaX = _endSizeX - _startSizeX;
    _deltaY = _endSizeY - _startSizeY;
}

void HBSizeTo::update(float time) {
    if ( _target ) {
        _target->setContentSize(Size(_startSizeX + _deltaX * time, _startSizeY + _deltaY * time));
    }
}
