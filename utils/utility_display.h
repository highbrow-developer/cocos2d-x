//
//  utility_display.h
//  cocos2d_libs
//
//  Created by 이재은 on 14. 8. 5..
//
//

#ifndef __cocos2d_libs__utility_display__
#define __cocos2d_libs__utility_display__

#include "cocos2d.h"

NS_CC_BEGIN

#define display_size                    Director::getInstance()->getWinSize()

#define display_center                  display_size / 2.0f
#define display_top                     Point(display_center.x, display_size.height)
#define display_bottom                  Point(display_center.x, 0.0f)

#define display_leftTop                 Point(0.0f, display_top.y)
#define display_left                    Point(0.0f, display_center.y)
#define display_leftBot                 Point(0.0f, 0.0f)

#define display_rightTop                display_size
#define display_right                   Point(display_right.x, display_center.y)
#define display_righyBot                Point(display_right.x, 0.0f)


class utility_display : public Object {
public:
//    static Size size();
};


NS_CC_END

#endif /* defined(__cocos2d_libs__utility_display__) */
