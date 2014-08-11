//
//  HBMenuItemImage.h
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 8..
//
//

#ifndef __cocos2d_libs__HBMenuItemImage__
#define __cocos2d_libs__HBMenuItemImage__

#include "cocos2d.h"

NS_CC_BEGIN

class CC_DLL HBMenuItemImage : public MenuItemImage {
public:
    HBMenuItemImage();
    ~HBMenuItemImage();
    
    static HBMenuItemImage* create();
    static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage);
    static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage);
    
    CC_DEPRECATED_ATTRIBUTE static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage, Ref* target, SEL_MenuHandler selector);
    
    static HBMenuItemImage* create(const std::string&normalImage, const std::string&selectedImage, const ccMenuCallback& callback);
    
    CC_DEPRECATED_ATTRIBUTE static HBMenuItemImage* create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, Ref* target, SEL_MenuHandler selector);
    
    static HBMenuItemImage* create(const std::string&normalImage, const std::string&selectedImage, const std::string&disabledImage, const ccMenuCallback& callback);
    
    virtual void activate();
    virtual void selected();
    virtual void unselected();
    
    void setActivateLong(bool value);
    void setSelectedLong(bool value);
    
    void setActivateAction(ActionInterval *action);
    void setSelectedAction(ActionInterval *action);
    
protected:
    void scheduleTimer(float timeScale_default, float timeScale_minimum);
    
public:
protected:
    bool _activateLong;
    bool _selectedLong;
    
    float _schedule_default;
    float _schedule_current;
    float _schedule_minimum;
    
    ActionInterval *_action_selected;
    ActionInterval *_action_unselected;
};

NS_CC_END

#endif /* defined(__cocos2d_libs__HBMenuItemImage__) */