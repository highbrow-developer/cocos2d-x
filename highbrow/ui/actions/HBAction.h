//
//  HBAction.h
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 22..
//
//

#ifndef __cocos2d_libs__HBAction__
#define __cocos2d_libs__HBAction__

#include "cocos2d.h"

NS_CC_BEGIN
//----------------------------------------
//                Shake
//----------------------------------------
class HBShake : public ActionInterval {
public:
    HBShake() { };
    virtual ~HBShake() { };
    
    static HBShake* create(float d, float strength );
    static HBShake* create(float d, float strength_x, float strength_y );
    
    virtual HBShake* clone() const override;
	virtual HBShake* reverse() const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    virtual void stop() override;
    
    
    bool initWithDuration(float d, float strength_x, float strength_y );
    
    
protected:
    float _initial_x, _initial_y;
    float _strength_x, _strength_y;
};


//----------------------------------------
//                SizeTo
//----------------------------------------
class HBSizeTo : public ActionInterval
{
public:
    HBSizeTo();
    virtual ~HBSizeTo() { };
    
    static HBSizeTo* create(float duration, Size size);
    static HBSizeTo* create(float duration, float width);
    
    virtual HBSizeTo* clone() const override;
	virtual HBSizeTo* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;
    
    bool initWithDuration(float duration, float width, float height = 0.0f);
    
protected:
    float _startSizeX;
    float _startSizeY;
    float _endSizeX;
    float _endSizeY;
    float _deltaX;
    float _deltaY;
};

NS_CC_END


#endif /* defined(__cocos2d_libs__HBAction__) */
