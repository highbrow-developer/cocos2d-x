//
//  Singleton.h
//
//  Created by 조 중욱 on 2014. 8. 22..
//
//

#ifndef __Singleton__
#define __Singleton__

template <typename T>
class Singleton
{
protected:
    static T* _instance;
    virtual void init() = 0;
public:
    static T* getInstance() {
        if(!_instance) {
            _instance = new T();
            _instance->init();
        }
        return _instance;
    }
    
    static void destroy() {
        if(_instance) {
            delete _instance;
            _instance = nullptr;
        }
    }
};

template <typename T> T* Singleton<T>::_instance = nullptr;

#endif