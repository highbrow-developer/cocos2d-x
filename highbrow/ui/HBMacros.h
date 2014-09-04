//
//  HBMacros.h
//  cocos2d_libs
//
//  Created by 이재은 on 2014. 8. 7..
//
//

#ifndef __cocos2d_libs__HBMacros__
#define __cocos2d_libs__HBMacros__

#include "cocos2d.h"


NS_CC_BEGIN

#define display_init(resolution)        HBMacros::setDesignResolution(resolution)

/*  화면 사이즈   */
#define display_size                    Size(Director::getInstance()->getWinSize())

/*  jpg ( bg ) 화면 맞춤  */
#define sinc_bg(node)                   HBMacros::setSincBackground(node)

/*  좌표 ( = VisibleRect )  */
#define pos_center                      Point(display_size / 2.0f)
#define pos_top                         Point(pos_center.x, display_size.height)
#define pos_bottom                      Point(pos_center.x, 0.0f)

#define pos_left                        Point(0.0f, pos_center.y)
#define pos_leftTop                     Point(0.0f, pos_top.y)
#define pos_leftBot                     Point(0.0f, 0.0f)

#define pos_right                       Point(display_size.width, pos_center.y)
#define pos_rightTop                    Point(display_size)
#define pos_rightBot                    Point(pos_right.x, 0.0f)

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


/*  싱글톤 매크로  */
#define single(class)                   class::getInstance()


/*  터치  */
#define lock(target)                    HBMacros::setTouchDisable(target)


/*  BM폰트 색상  */
#define Color_BMF(label, str)           HBMacros::setColorForBMFont(label, str)


class HBMacros : public Ref {
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
    
    static void setSincBackground(Node *node) {
//        node->setScaleX(display_size.width / node->getContentSize().width);
//        node->setScaleY(display_size.height / node->getContentSize().height);
        node->setScale(display_size.width / node->getContentSize().width);
    }
    
    /*  화면 터치 잠금  */
    static void setTouchDisable(Node *target) {
        auto touch = EventListenerTouchOneByOne::create();
        touch->onTouchBegan = [=](Touch *touch, Event *event) { return true; };
        touch->setSwallowTouches(true);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, target);
    }
    
    /*  BM폰트 색상 변경  */
    static void setColorForBMFont(LabelBMFont *label, std::string str) {
        
        std::vector<int> vec_pos;
        std::vector<std::string> vec_hex;
        
        int pos = 0;
        while ( str.find('{', pos) != -1 ) {
            pos = str.find('{', pos);
            if ( str[pos + 1] == '#' && str.find('}',pos) != -1 ) {
                int line = 0;
                std::string str_sub = str.substr(0, pos);
                
                for ( int i = 0; i < str_sub.length(); ++i ) {
                    short c = str_sub[i];
                    if ( c == '\n' ) {
                        line++;
                    }
                }
                
                vec_hex.push_back(str.substr(pos+2, 6));
                str.erase(pos, 9);
                
                int pos_start = pos + line;
                
                pos = str.find('}', pos);
                line = 0;
                str_sub = str.substr(0, pos);
                
                for ( int i = 0; i < str_sub.length(); ++i ) {
                    short c = str_sub[i];
                    if (c == '\n') {
                        line++;
                    }
                }
                
                str.erase(pos, 1);
                
                int pos_end     = pos + line;
                int pos_kr      = 0;
                
                bool set_start  = false;
                
                for ( int i = 0; i < pos_end; i++ ) {
                    if ( i >= pos_start && !set_start ) {
                        pos_start = pos_kr;
                        set_start = true;
                    }
                    
                    pos_kr++;
                    if ( str[i] & 0x80 )    i += 2;
                }
                
                pos_end = pos_kr;
                
                vec_pos.push_back(pos_start);
                vec_pos.push_back(pos_end);
            }
        }
        
        label->setString(str.c_str());
        
        for ( int i = 0; i < vec_hex.size(); i++ ) {
            std::string str_hex = vec_hex[i];
            
            int rgb[3] = { 0, 0, 0 };
            
            {
                std::stringstream ss_r;
                ss_r<<std::hex<<str_hex.substr(0,2);
                ss_r>>rgb[0];
                
                std::stringstream ss_g;
                ss_g<<std::hex<<str_hex.substr(2,2);
                ss_g>>rgb[1];
                
                std::stringstream ss_b;
                ss_b<<std::hex<<str_hex.substr(4,2);
                ss_b>>rgb[2];
            }
            
            for ( int j = vec_pos[i * 2]; j < vec_pos[i * 2 + 1]; j++ ) {
                auto label_sprite = (Sprite*)label->getChildByTag(j);
                if ( label_sprite != NULL ) {
                    label_sprite->setColor(Color3B(rgb[0], rgb[1], rgb[2]));
                }
            }
        }
    }
    
    static Layer* getClippingSpriteForSkill(const char* filename) {
        Layer *pLayer = Layer::create();
        
        ClippingNode *clipper = ClippingNode::create();
        clipper->setPosition( Point::ZERO );
        pLayer->addChild(clipper);
        
        auto content = Sprite::createWithSpriteFrameName(filename);
        content->setPosition( Point::ZERO );
        clipper->addChild(content);
        
        DrawNode *stencil = DrawNode::create();
        
        std::vector<Vec2> vertex;
        float precision = .5f;
        float cir = 2 * M_PI;
        float radius = content->getContentSize().width / 2 - 2;
        
        
        for (float a = .0f; a < cir; a += precision) {
            float x = radius * cos(a);
            float y = radius * sin(a);
            
            vertex.push_back(Vec2(x,y));
        }
        stencil->drawPolygon(&vertex[0], vertex.size(), Color4F(0,0,0,0), 2, Color4F(1,1,1,1));
        clipper->setStencil(stencil);
        
        
        return pLayer;
    };
};


NS_CC_END
#endif /* defined(__cocos2d_libs__HBMacros__) */
