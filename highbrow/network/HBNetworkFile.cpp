//
//  HBNetworkFile.cpp
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 22..
//
//

#include "HBNetworkFile.h"
#include "unzip.h"

#include <sys/stat.h>

NS_CC_BEGIN

static std::mutex       s_requestFileQueueMutex;
static std::mutex       s_responseFileQueueMutex;

static std::mutex       s_SleepDownMutex;
static std::condition_variable      s_SleepDownCondition;

static std::mutex       s_SleepZipMutex;
static std::condition_variable      s_SleepZipCondition;

static bool s_need_down_quit = false;
static bool s_need_zip_quit = false;
static bool s_need_error = false;

static Vector<__String*>* s_requestFileQueue = nullptr;
static Vector<__String*>* s_responseFileQueue = nullptr;

HBNetworkFile* HBNetworkFile::s_pHBNetworkFile = nullptr;
std::string HBNetworkFile::s_strFileSavePath = "";
std::string HBNetworkFile::s_strFileSaveFile = "";
HBNetworkFile::HBNetworkFile() :
_nListTotal(0),
_nListNow(0),
_callbackProgress(nullptr),
_callbackEnd(nullptr)
{
    s_need_down_quit = false;
    s_need_zip_quit = false;
}

HBNetworkFile::~HBNetworkFile()
{
    s_need_down_quit = true;
    s_need_zip_quit = true;
    
    if ( s_requestFileQueue != nullptr )
    {
        s_SleepDownCondition.notify_one();
    }
    
    if ( s_responseFileQueue != nullptr )
    {
        s_SleepZipCondition.notify_one();
    }
    
    _callbackProgress = nullptr;
    _callbackEnd = nullptr;
    
    s_strFileSavePath = "";
    s_strFileSaveFile = "";
    s_pHBNetworkFile = nullptr;
}

bool HBNetworkFile::init()
{
    // 파일 리스트
    if ( s_requestFileQueue == nullptr )
        s_requestFileQueue = new Vector<__String*>();
    
    if ( s_responseFileQueue == nullptr )
        s_responseFileQueue = new Vector<__String*>();
    
    // 쓰레드
    auto tFile = std::thread(CC_CALLBACK_0(HBNetworkFile::networkFileThread, this));
    tFile.detach();
    
    auto tZip = std::thread(CC_CALLBACK_0(HBNetworkFile::networkZipThread, this));
    tZip.detach();
    
    // 파일 저장 경로
    s_strFileSavePath = FileUtils::getInstance()->getWritablePath();

    // 파일 저장 경로 폴더생성
    int pos = 0;
    while( (pos = s_strFileSavePath.find_first_of( "/\\", pos )) != std::string::npos )
    {
        std::string subPath = s_strFileSavePath.substr( 0, pos++ );
        mkdir(subPath.c_str(), 0777);
    }
    
    return true;
}

void HBNetworkFile::destroyInstance()
{
    CC_SAFE_DELETE(s_pHBNetworkFile);
}

#pragma mark - set
void HBNetworkFile::setFileSavePath(std::string strPath)
{
    s_strFileSavePath = strPath;
    
    //파일 저장 경로 폴더생성
    int pos = 0;
    while( (pos = s_strFileSavePath.find_first_of( "/\\", pos )) != std::string::npos )
    {
        std::string subPath = s_strFileSavePath.substr( 0, pos++ );
        mkdir(subPath.c_str(), 0777);
    }
}

void HBNetworkFile::setFileProgress(const std::function<void(int, int, float)>& callback)
{
    _callbackProgress = callback;
}

void HBNetworkFile::setFileEnd(const std::function<void(int)>& callback)
{
    _callbackEnd = callback;
}

#pragma mark - download
void HBNetworkFile::download(Vector<__String*> &list)
{
    if ( s_requestFileQueue != nullptr )
    {
        s_need_down_quit = false;
        s_need_error = false;
        
        // 다운로드 수
        _nListTotal = list.size();
        _nListNow = 1;
        
        // 다운로드 파일 추가
        s_requestFileQueueMutex.lock();
        s_requestFileQueue->pushBack(list);
        s_requestFileQueueMutex.unlock();
        
        // 다운로드 시작
        s_SleepDownCondition.notify_one();
    }
}

void HBNetworkFile::downloadStart()
{
    if ( s_strFileSaveFile.length() !=  0 )
    {
        downloadTask(s_strFileSaveFile, s_strFileSavePath);
    }
}

void HBNetworkFile::downloadEnd()
{
    // 다운로드 완료 후 압축 해제
    s_requestFileQueueMutex.lock();
    if ( s_requestFileQueue->empty() )
    {
        s_SleepZipCondition.notify_one();
    }
    s_requestFileQueueMutex.unlock();
    
    // 다운로드 시작 및 대기
    _nListNow++;
    s_SleepDownCondition.notify_one();
}

void HBNetworkFile::downloadError()
{
    // 오류 처리
    s_need_error = true;
    
    s_requestFileQueueMutex.lock();
    s_requestFileQueue->clear();
    s_requestFileQueueMutex.unlock();
    
    s_responseFileQueueMutex.lock();
    s_responseFileQueue->clear();
    s_responseFileQueueMutex.unlock();
    
    if ( s_pHBNetworkFile != nullptr && _callbackEnd != nullptr )
    {
        s_pHBNetworkFile->_callbackEnd(1);
    }
}

void HBNetworkFile::downloadNow(float nProgress)
{
    if ( s_pHBNetworkFile != nullptr && _callbackProgress != nullptr )
    {
        s_pHBNetworkFile->_callbackProgress(_nListTotal, _nListNow, nProgress);
    }
}

#pragma mark - thread
void HBNetworkFile::networkFileThread()
{
    auto scheduler = Director::getInstance()->getScheduler();
    
    while (true)
    {
        if (s_need_down_quit)
        {
            break;
        }
        
        s_strFileSaveFile = "";
        s_requestFileQueueMutex.lock();
        if ( !s_requestFileQueue->empty() )
        {
            s_strFileSaveFile = s_requestFileQueue->at(0)->getCString();
            s_requestFileQueue->erase(0);
        }
        s_requestFileQueueMutex.unlock();
     
        if ( s_strFileSaveFile.length() == 0 )
        {
            // 다운로드 파일이 없을때 일시 정지
            std::unique_lock<std::mutex> lk(s_SleepDownMutex);
            s_SleepDownCondition.wait(lk);
            continue;
        }
        
        s_responseFileQueueMutex.lock();
        s_responseFileQueue->pushBack(__String::create(s_strFileSaveFile));
        s_responseFileQueueMutex.unlock();
        
        std::string strFileName = s_strFileSaveFile.substr( s_strFileSaveFile.find_last_of("/\\") + 1, s_strFileSaveFile.length() - s_strFileSaveFile.find_last_of("/\\"));
        std::string strFile = s_strFileSavePath + strFileName;
        if ( access(strFile.c_str(), F_OK) == 0 )
        {
            downloadEnd();
            continue;
        }
        
        // 다운로드 시작
        if ( nullptr != s_pHBNetworkFile )
        {
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HBNetworkFile::downloadStart, this));
        }
        
        // 다운로드 중 일시 정지
        std::unique_lock<std::mutex> lk(s_SleepDownMutex);
        s_SleepDownCondition.wait(lk);
    }
    
    s_requestFileQueueMutex.lock();
    s_requestFileQueue->clear();
    s_requestFileQueueMutex.unlock();
}

void HBNetworkFile::networkZipThread()
{
    auto scheduler = Director::getInstance()->getScheduler();
    
    while (true)
    {
        if (s_need_zip_quit)
        {
            break;
        }
        
        std::string request = "";
        
        s_responseFileQueueMutex.lock();
        if ( !s_responseFileQueue->empty() )
        {
            request = s_responseFileQueue->at(0)->getCString();
            s_responseFileQueue->erase(0);
        }
        s_responseFileQueueMutex.unlock();
        
        if ( request.length() == 0 )
        {
            // Wait for http request tasks from main thread
            std::unique_lock<std::mutex> lk(s_SleepZipMutex);
            s_SleepZipCondition.wait(lk);
            continue;
        }
        
        unsigned char * pBuffer = NULL;
        unzFile pFile = NULL;
        struct stat st;
        long long nSize = 0;
        std::string strFileName = request.substr( request.find_last_of("/\\") + 1, request.length() - request.find_last_of("/\\"));;
        std::string strFilePath = s_strFileSavePath;
        std::string strZipFile = s_strFileSavePath + strFileName;
        stat(strZipFile.c_str(), &st);
        
        //폴더생성
        int pos = 0;
        while( (pos = strFilePath.find_first_of( "/\\", pos )) != std::string::npos )
        {
            std::string subPath = strFilePath.substr( 0, pos++ );
            
            mkdir(subPath.c_str(), 0777);
        }

        if ( s_pHBNetworkFile != nullptr )
        {
            nSize = s_pHBNetworkFile->getMemorySize();
        }
        
        pFile = unzOpen(strZipFile.c_str());
        if ( pFile != NULL && getMemorySize() > st.st_size * 1.5f )
        {
            unzGoToFirstFile(pFile);
            
            unz_global_info global_info;
            unzGetGlobalInfo( pFile, &global_info );
            
            uLong i;
            for ( i = 0; i < global_info.number_entry; ++i )
            {
                char tmpFileName[260];
                unz_file_info FileInfo;
                
                int nRet = unzGetCurrentFileInfo(pFile, &FileInfo, tmpFileName, sizeof(tmpFileName), NULL, 0, NULL, 0);
                if ( nRet != UNZ_OK )
                    continue;
                
                //폴더 확인
                std::string strFile = strFilePath + tmpFileName;
                const size_t tmpFileLength = strlen( tmpFileName );
                if ( tmpFileName[ tmpFileLength-1 ] == '/' )
                {
                    mkdir(strFile.c_str(), 0777);
                }
                else
                {
                    nRet = unzOpenCurrentFile(pFile);
                    if ( nRet != UNZ_OK )
                        continue;
                    
                    FILE *pFileOut = fopen( strFile.c_str(), "wb" );
                    if ( pFileOut != NULL )
                    {
                        int nSize = 0;
                        pBuffer = new unsigned char[FileInfo.uncompressed_size];
                        nSize = unzReadCurrentFile(pFile, pBuffer, FileInfo.uncompressed_size);
                        if ( nSize > 0 )
                        {
                            fwrite( pBuffer, nSize, 1, pFileOut );
                            fclose( pFileOut );
                        }
                        delete pBuffer;
                    }
                    
                    unzCloseCurrentFile(pFile);
                }
                
                //다음파일
                if ( ( i+1 ) < global_info.number_entry )
                {
                    if ( unzGoToNextFile( pFile ) != UNZ_OK )
                    {
                        break;
                    }
                }
            }
            
            unzClose(pFile);
            
            //파일 삭제
            remove(strZipFile.c_str());
            
            if ( s_responseFileQueue->empty() == true && s_pHBNetworkFile != nullptr && _callbackEnd != nullptr && s_need_error == false )
            {
                s_pHBNetworkFile->_callbackEnd(0);
            }
        }
        else
        {
            if ( nullptr != s_pHBNetworkFile )
            {
                scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HBNetworkFile::downloadError, this));
            }
        }
    }
    
    s_responseFileQueueMutex.lock();
    s_responseFileQueue->clear();
    s_responseFileQueueMutex.unlock();
}

NS_CC_END
