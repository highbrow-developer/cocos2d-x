//
//  HBMacros.h
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 7..
//
//

#ifndef __cocos2d_libs__HBmacros__
#define __cocos2d_libs__HBmacros__

#include "cocos2d.h"


NS_CC_BEGIN

#define display_init(resolution)        HBmacros::setDesignResolution(resolution)

/*  화면 사이즈   */
#define display_size                    Size(Director::getInstance()->getWinSize())

/*  좌표 ( = VisibleRect )  */
#define pos_center                      display_size / 2.0f
#define pos_top                         Point(pos_center.x, pos_size.height)
#define pos_bottom                      Point(pos_center.x, 0.0f)

#define pos_left                        Point(0.0f, pos_center.y)
#define pos_leftTop                     Point(0.0f, pos_top.y)
#define pos_leftBot                     Point(0.0f, 0.0f)

#define pos_right                       Point(pos_right.x, pos_center.y)
#define pos_rightTop                    display_size
#define pos_righyBot                    Point(pos_right.x, 0.0f)

/*  앵커  */
#define anc_center                      Point(0.5f, 0.5f)
#define anc_top                         Point(anc_center.x, 1.0f)
#define anc_bottom                      Point(anc_center.x, 0.0f)

#define anc_left                        Point(0.0f, anc_center.y)
#define anc_leftTop                     Point(anc_left.x, anc_top.y)
#define anc_leftBot                     Point(anc_left.x, anc_bottom.y)

#define anc_right                       Point(0.0f, anc_center.y)
#define anc_rightTop                    Point(anc_right.x, anc_top.y)
#define anc_rightBot                    Point(anc_right.x, anc_bottom.y)



/*  터치  */
#define touch_graph_lock(target)        HBmacros::setTouchGraphLock(target);



class HBmacros : public Ref {
public:
    /*  디자인 해상도  */
    static void setDesignResolution(Size resolution) {
        auto director   = Director::getInstance();
        auto glview     = director->getOpenGLView();
        if ( glview == NULL ) {
            glview = GLView::create("MonsterVillage");
        }
        
        typedef struct tagResource {
            cocos2d::Size size;
            char directory[100];
        } resource;
        
        static resource resource_320    =  { cocos2d::Size(512.0f,  346.0f), "320" };
        static resource resource_480    =  { cocos2d::Size(768.0f,  519.0f), "480" };
        static resource resource_640    =  { cocos2d::Size(1024.0f, 692.0f), "640" };
        
        auto size_frame = glview->getFrameSize();
        
        if ( size_frame.height > 480.0f ) {
            FileUtils::getInstance()->addSearchPath(resource_640.directory);
            director->setContentScaleFactor(resource_640.size.height / resolution.height);
        }
        else if ( size_frame.height > 320.0f ) {
            FileUtils::getInstance()->addSearchPath(resource_480.directory);
            director->setContentScaleFactor(resource_480.size.height / resolution.height);
        }
        else {
            FileUtils::getInstance()->addSearchPath(resource_320.directory);
            director->setContentScaleFactor(resource_320.size.height / resolution.height);
        }
        
        resolution = Size(size_frame.width * (resolution.height / size_frame.height), resolution.height);//692.0f
        glview->setDesignResolutionSize(resolution.width, resolution.height, ResolutionPolicy::NO_BORDER);
        
        director->setOpenGLView(glview);
    }
    
    
    /*  화면 터치 잠금  */
    static void setTouchGraphLock(Node *target) {
        auto touch = EventListenerTouchOneByOne::create();
        touch->onTouchBegan = [=](Touch *touch, Event *event) { return false; };
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, target);
//        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(target);
    }
};


NS_CC_END
#endif /* defined(__cocos2d_libs__HBMacros__) */
