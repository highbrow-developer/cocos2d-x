//
//  HBMenuItemImage.cpp
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 8..
//
//

#include "HBMenuItemImage.h"

USING_NS_CC;


HBMenuItemImage::HBMenuItemImage() :
_activateLong(false),
_selectedLong(false),
_schedule_default(0.2f),
_schedule_current(0.2f),
_schedule_minimum(0.2f),
_action_selected(NULL),
_action_unselected(NULL) {
    
}

HBMenuItemImage::~HBMenuItemImage() {
    if ( _action_selected != NULL )     CC_SAFE_RELEASE_NULL(_action_selected);
    if ( _action_unselected != NULL )   CC_SAFE_RELEASE_NULL(_action_unselected);
}


HBMenuItemImage* HBMenuItemImage::create()
{
    HBMenuItemImage *ret = new HBMenuItemImage();
    if ( ret && ret->initWithNormalImage("", "", "", (const ccMenuCallback&)nullptr) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

HBMenuItemImage * HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage)
{
    return HBMenuItemImage::create(normalImage, selectedImage, "", (const ccMenuCallback&)nullptr);
}

// XXX deprecated
HBMenuItemImage * HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, Ref* target, SEL_MenuHandler selector)
{
    return HBMenuItemImage::create(normalImage, selectedImage, "", target, selector);
}

HBMenuItemImage * HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback)
{
    return HBMenuItemImage::create(normalImage, selectedImage, "", callback);
}

// XXX deprecated
HBMenuItemImage * HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, Ref* target, SEL_MenuHandler selector)
{
    HBMenuItemImage *ret = new HBMenuItemImage();
    if ( ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, target, selector) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

HBMenuItemImage * HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback)
{
    HBMenuItemImage *ret = new HBMenuItemImage();
    if ( ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

HBMenuItemImage * HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage)
{
    HBMenuItemImage *ret = new HBMenuItemImage();
    if ( ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, (const ccMenuCallback&)nullptr) )
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void HBMenuItemImage::activate() {
    if ( this->isEnabled() ) {
        if ( _action_selected != NULL ) {
            this->getActionManager()->removeAction(_action_selected);
            this->runAction(_action_selected);
        }
    }
}