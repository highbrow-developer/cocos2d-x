//
//  HBNetworkFile.h
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 22..
//
//

#ifndef __cocos2d_libs__HBNetworkFile__
#define __cocos2d_libs__HBNetworkFile__

#include "cocos2d.h"

NS_CC_BEGIN

class CC_DLL HBNetworkFile {
public:
    static HBNetworkFile* getInstance();

    static void destroyInstance();
    
    /** The default constructor. */
    HBNetworkFile(void);
    
    /** The default destructor */
    virtual ~HBNetworkFile(void);
    
    /** Initializes the HBNetworkFile */
    virtual bool init();
    
protected:
    
    
public:
    /** 파일 설치 경로 설정 */
    void setFileSavePath(std::string strPath);
    
    /** 파일 다운로드 과정 설정 */
    void setFileProgress(const std::function<void(int, int, float)>& callback);
    
    /** 파일 다운로드 결과 설정 */
    void setFileEnd(const std::function<void(int)>& callback);
    
    /** 메모리 용량 */
    virtual long long getMemorySize() = 0;
    
    /** 다운로드 */
    void download(Vector<__String*> &list);
    void downloadStart();
    void downloadEnd();
    void downloadError();
    void downloadNow(float nProgress);
    
    virtual void downloadTask(std::string strUrl, std::string strPath) = 0;
    
    /** 쓰레드 */
    void networkFileThread();
    void networkZipThread();
    
private:
    
protected:
    /** 다운로드 결과*/
    int _nListTotal;
    int _nListNow;
    
    /** 다운로드 과정 및 결과 */
    std::function<void(int, int, float)> _callbackProgress;   //전체, 현재, 퍼센트
    std::function<void(int)> _callbackEnd;

    /** 파일 경로 */
    static std::string s_strFileSavePath;
    static std::string s_strFileSaveFile;
    
    /** */
    static HBNetworkFile *s_pHBNetworkFile;
};



NS_CC_END

#endif /* defined(__cocos2d_libs__HBNetworkFile__) */
