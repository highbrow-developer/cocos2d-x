//
//  HBLogin.h
//  HBLogin
//
//  Created by Heewon Song on 2014. 8. 21..
//
//
#ifndef __HB_LOGIN_H__
#define __HB_LOGIN_H__

#include <functional>

class HBLogin
{
public:
    static void init(const char *gameId, const char *languageCode);
    static void showLoginPopup(const std::function<void(const char *)> &callback);
//    static void logout();
//    static void signup();
    static void hideLoadingLayer();
    
};

#endif
