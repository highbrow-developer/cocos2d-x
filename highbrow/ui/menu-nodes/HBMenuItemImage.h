//
//  HBMenuItemImage.h
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 14..
//
//

#ifndef __cocos2d_libs__HBMenuItemImage__
#define __cocos2d_libs__HBMenuItemImage__

#include "cocos2d.h"

NS_CC_BEGIN

class HBMenuItemImage : public MenuItemImage {
public:
    HBMenuItemImage();
    virtual ~HBMenuItemImage();
    
    static HBMenuItemImage* create();
    static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage);
    static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage);
    static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback);
    static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback);
    
protected:
    virtual void activate();
    virtual void selected();
    virtual void unselected();
    
public:
    /*  크기 저장용  */
    virtual void setScale(float scale);
    float getScaleSetting() { return _scale_setting; };
    
    void setActivateLong(bool value);
    void setSelectedLong(bool value);
    
    void setActivateAction(ActionInterval *action);
    void setSelectedAction(ActionInterval *action);
    void setUnselectedAction(ActionInterval *action);
    
protected:
    void scheduleTimer(float timeScale_default, float timeScale_minimum);
    
public:
protected:
    
    float _scale_setting;
    
    bool _activateLong;
    bool _selectedLong;
    
    float _schedule_default;
    float _schedule_current;
    float _schedule_minimum;
    
    ActionInterval *_action_activate;
    ActionInterval *_action_selected;
    ActionInterval *_action_unselected;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__HBMenuItemImage__) */
