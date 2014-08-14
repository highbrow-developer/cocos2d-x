//
//  HBMenuItemImage.cpp
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 14..
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
_action_activate(NULL),
_action_selected(NULL),
_action_unselected(NULL) {
    
}

HBMenuItemImage::~HBMenuItemImage() {
    if ( _action_activate != NULL )     CC_SAFE_RELEASE_NULL(_action_activate);
    if ( _action_selected != NULL )     CC_SAFE_RELEASE_NULL(_action_selected);
    if ( _action_unselected != NULL )   CC_SAFE_RELEASE_NULL(_action_unselected);
}

HBMenuItemImage* HBMenuItemImage::create() {
    HBMenuItemImage *ret = new HBMenuItemImage();
    if ( ret && ret->initWithNormalImage("", "", "", (const ccMenuCallback&)nullptr) ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

HBMenuItemImage* HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage) {
    return HBMenuItemImage::create(normalImage, selectedImage, "", (const ccMenuCallback&)nullptr);
}

HBMenuItemImage* HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const ccMenuCallback& callback) {
    return HBMenuItemImage::create(normalImage, selectedImage, "", callback);
}

HBMenuItemImage* HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage, const ccMenuCallback& callback) {
    HBMenuItemImage *ret = new HBMenuItemImage();
    if ( ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, callback) ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

HBMenuItemImage * HBMenuItemImage::create(const std::string& normalImage, const std::string& selectedImage, const std::string& disabledImage) {
    HBMenuItemImage *ret = new HBMenuItemImage();
    if ( ret && ret->initWithNormalImage(normalImage, selectedImage, disabledImage, (const ccMenuCallback&)nullptr) ) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void HBMenuItemImage::activate() {
    if ( this->isEnabled() ) {
        this->stopAllActions();
        if ( _action_activate == NULL ) {
            this->runAction(Sequence::create(ScaleBy::create(0.075f, 1.0f, 1.125f),
                                             ScaleBy::create(0.075f, 1.125f, 1.0f),
                                             ScaleBy::create(0.075f, 1.0f),
                                             DelayTime::create(0.05f),
                                             CallFunc::create( [=](void) { MenuItem::activate();} ),
                                             NULL));
        }
        else {
            this->runAction(Sequence::create(_action_activate->clone(),
                                             DelayTime::create(0.05f),
                                             CallFunc::create( [=](void) { MenuItem::activate();} ),
                                             NULL));
        }
        
    }
}

void HBMenuItemImage::selected() {
    if ( this->isEnabled() ) {
        this->stopAllActions();
        if ( _action_selected == NULL ) {
            this->runAction(EaseIn::create(ScaleTo::create(0.5f, 1.1f), 0.25f));
        }
        else {
            this->runAction(_action_selected->clone());
        }
        MenuItem::selected();
    }
}

void HBMenuItemImage::unselected() {
    if ( this->isEnabled() ) {
        this->stopAllActions();
        if ( _action_unselected == NULL ) {
            this->runAction(Spawn::create(EaseIn::create(ScaleTo::create(0.25f, 1.0f), 0.25f),
                                          FadeTo::create(0.25f, 255.0f),
                                          NULL));
        }
        else {
            this->runAction(_action_unselected->clone());
        }
        MenuItem::unselected();
    }
}


void HBMenuItemImage::setActivateAction(ActionInterval *action) {
    _action_activate = action;
}

void HBMenuItemImage::setSelectedAction(ActionInterval *action) {
    _action_selected = action;
    _action_selected->retain();
}

void HBMenuItemImage::setUnselectedAction(ActionInterval *action) {
    _action_unselected = action;
}