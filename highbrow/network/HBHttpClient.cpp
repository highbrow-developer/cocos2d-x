/****************************************************************************
 Copyright (c) 2012      greathqy
 Copyright (c) 2012      cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HBHttpClient.h"

#include <thread>
#include <queue>
#include <condition_variable>

#include <errno.h>

#include "base/CCVector.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"

#include "curl/curl.h"

#include "platform/CCFileUtils.h"

#include <sys/stat.h>

NS_CC_BEGIN

namespace hbnetwork {

static std::mutex       s_requestQueueMutex;
static std::mutex       s_responseQueueMutex;

static std::mutex       s_SleepMutex;
static std::condition_variable      s_SleepCondition;


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
typedef int int32_t;
#endif

static bool s_need_quit = false;

static Vector<HBHttpRequest*>*  s_requestQueue = nullptr;
static Vector<HBHttpResponse*>* s_responseQueue = nullptr;

static HBHttpClient *s_pHBHttpClient = nullptr; // pointer to singleton
HBHttpRequest *s_request = NULL;
static float s_nProgress = 0.f;

static char s_errorBuffer[CURL_ERROR_SIZE] = {0};

typedef size_t (*write_callback)(void *ptr, size_t size, size_t nmemb, void *stream);
typedef int (*progress_callback)(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);

static std::string s_cookieFilename = "";

// Callback function used by libcurl for collect response data
static size_t writeData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}
    
static size_t writeDataFile(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *fp = (FILE*)stream;
    size_t written = fwrite(ptr, size, nmemb, fp);
    
    return written;
}

// Callback function used by libcurl for collect header data
static size_t writeHeaderData(void *ptr, size_t size, size_t nmemb, void *stream)
{
    std::vector<char> *recvBuffer = (std::vector<char>*)stream;
    size_t sizes = size * nmemb;
    
    // add data to the end of recvBuffer
    // write data maybe called more than once in a single request
    recvBuffer->insert(recvBuffer->end(), (char*)ptr, (char*)ptr+sizes);
    
    return sizes;
}

static int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    if(totalToDownload <= 0) {
        s_nProgress = 0.f;
    } else {
        s_nProgress = (float)(nowDownloaded/totalToDownload*100);
    }
    return 0;
}
    

static int processGetTask(HBHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
static int processGetFileTask(HBHttpRequest *request, write_callback callback, progress_callback progresscallback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream);
static int processPostTask(HBHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
static int processPutTask(HBHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
static int processDeleteTask(HBHttpRequest *request, write_callback callback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream, char *errorBuffer);
// int processDownloadTask(HBHttpRequest *task, write_callback callback, void *stream, int32_t *errorCode);
static void processResponse(HBHttpResponse* response, char* errorBuffer);

// Worker thread
void HBHttpClient::networkThread()
{
    s_request = nullptr;
    //HBHttpRequest *request = nullptr;
    
    auto scheduler = Director::getInstance()->getScheduler();
    
    while (true) 
    {
        if (s_need_quit)
        {
            break;
        }
        
        // step 1: send http request if the requestQueue isn't empty
        s_request = nullptr;
        
        s_requestQueueMutex.lock();
        
        //Get request task from queue
        
        if (!s_requestQueue->empty())
        {
            s_request = s_requestQueue->at(0);
            s_requestQueue->erase(0);
        }
        
        s_requestQueueMutex.unlock();
        
        if (nullptr == s_request)
        {
            // Wait for http request tasks from main thread
            std::unique_lock<std::mutex> lk(s_SleepMutex); 
            s_SleepCondition.wait(lk);
            continue;
        }
        
        // step 2: libcurl sync access
        
        // Create a HBHttpResponse object, the default setting is http access failed
        HBHttpResponse *response = new HBHttpResponse(s_request);
        
        processResponse(response, s_errorBuffer);
        

        // add response packet into queue
        s_responseQueueMutex.lock();
        s_responseQueue->pushBack(response);
        s_responseQueueMutex.unlock();
        
        if (nullptr != s_pHBHttpClient) {
            scheduler->performFunctionInCocosThread(CC_CALLBACK_0(HBHttpClient::dispatchResponseCallbacks, this));
        }
    }
    
    // cleanup: if worker thread received quit signal, clean up un-completed request queue
    s_requestQueueMutex.lock();
    s_requestQueue->clear();
    s_requestQueueMutex.unlock();
    
    
    if (s_requestQueue != nullptr) {
        delete s_requestQueue;
        s_requestQueue = nullptr;
        delete s_responseQueue;
        s_responseQueue = nullptr;
    }

}

// Worker thread
void HBHttpClient::networkThreadAlone(HBHttpRequest* request)
{
    // Create a HBHttpResponse object, the default setting is http access failed
    HBHttpResponse *response = new HBHttpResponse(request);
    char errorBuffer[CURL_ERROR_SIZE] = { 0 };
    processResponse(response, errorBuffer);

    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->performFunctionInCocosThread([response, request]{
        const ccHttpRequestCallback& callback = request->getCallback();
        Ref* pTarget = request->getTarget();
        SEL_HBHttpResponse pSelector = request->getSelector();

        if (callback != nullptr)
        {
            callback(s_pHBHttpClient, response);
        }
        else if (pTarget && pSelector)
        {
            (pTarget->*pSelector)(s_pHBHttpClient, response);
        }
        response->release();
        // do not release in other thread
        request->release();
    });
}

//Configure curl's timeout property
static bool configureCURL(CURL *handle, char *errorBuffer)
{
    if (!handle) {
        return false;
    }
    
    int32_t code;
    code = curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, errorBuffer);
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_TIMEOUT, HBHttpClient::getInstance()->getTimeoutForRead());
    if (code != CURLE_OK) {
        return false;
    }
    code = curl_easy_setopt(handle, CURLOPT_CONNECTTIMEOUT, HBHttpClient::getInstance()->getTimeoutForConnect());
    if (code != CURLE_OK) {
        return false;
    }
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // FIXED #3224: The subthread of CCHBHttpClient interrupts main thread if timeout comes.
    // Document is here: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html#CURLOPTNOSIGNAL 
    curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1L);

    return true;
}

class CURLRaii
{
    /// Instance of CURL
    CURL *_curl;
    /// Keeps custom header data
    curl_slist *_headers;
public:
    CURLRaii()
        : _curl(curl_easy_init())
        , _headers(nullptr)
    {
    }

    ~CURLRaii()
    {
        if (_curl)
            curl_easy_cleanup(_curl);
        /* free the linked list for header data */
        if (_headers)
            curl_slist_free_all(_headers);
    }

    template <class T>
    bool setOption(CURLoption option, T data)
    {
        return CURLE_OK == curl_easy_setopt(_curl, option, data);
    }

    /**
     * @brief Inits CURL instance for common usage
     * @param request Null not allowed
     * @param callback Response write callback
     * @param stream Response write stream
     */
    bool init(HBHttpRequest *request, write_callback callback, void *stream, write_callback headerCallback, void *headerStream, char *errorBuffer)
    {
        if (!_curl)
            return false;
        if (!configureCURL(_curl, errorBuffer))
            return false;

        /* get custom header data (if set) */
        std::vector<std::string> headers=request->getHeaders();
        if(!headers.empty())
        {
            /* append custom headers one by one */
            for (std::vector<std::string>::iterator it = headers.begin(); it != headers.end(); ++it)
                _headers = curl_slist_append(_headers,it->c_str());
            /* set custom headers for curl */
            if (!setOption(CURLOPT_HTTPHEADER, _headers))
                return false;
        }
        if (!s_cookieFilename.empty()) {
            if (!setOption(CURLOPT_COOKIEFILE, s_cookieFilename.c_str())) {
                return false;
            }
            if (!setOption(CURLOPT_COOKIEJAR, s_cookieFilename.c_str())) {
                return false;
            }
        }

        return setOption(CURLOPT_URL, request->getUrl())
                && setOption(CURLOPT_WRITEFUNCTION, callback)
                && setOption(CURLOPT_WRITEDATA, stream)
                && setOption(CURLOPT_HEADERFUNCTION, headerCallback)
                && setOption(CURLOPT_HEADERDATA, headerStream);
        
    }

    /// @param responseCode Null not allowed
    bool perform(long *responseCode)
    {
        if (CURLE_OK != curl_easy_perform(_curl))
            return false;
        CURLcode code = curl_easy_getinfo(_curl, CURLINFO_RESPONSE_CODE, responseCode);
        if (code != CURLE_OK || !(*responseCode >= 200 && *responseCode < 300)) {
            CCLOGERROR("Curl curl_easy_getinfo failed: %s", curl_easy_strerror(code));
            return false;
        }
        // Get some mor data.
        
        return true;
    }
};

//Process Get Request
static int processGetTask(HBHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}
    
//Process Get File Request
static int processGetFileTask(HBHttpRequest *request, write_callback callback, progress_callback progresscallback, void *stream, long *errorCode, write_callback headerCallback, void *headerStream)
{
    Director::getInstance()->getScheduler()->scheduleSelector(schedule_selector(HBHttpClient::dispatchProgressCallbacks), HBHttpClient::getInstance(), 0, false);
    
    //파일위치
    if(strcmp(s_filePath.c_str(), "") == 0) {
        s_filePath = FileUtils::getInstance()->getWritablePath();
    }
    std::string strFileName = request->getUrl();
    strFileName = strFileName.substr( strFileName.find_last_of("/\\") + 1, strFileName.length() - strFileName.find_last_of("/\\"));
    //s_filePath.append(strFileName);
    
    //폴더생성
    int pos = 0;
    while( (pos = s_filePath.find_first_of( "/\\", pos )) != std::string::npos )
    {
        std::string subPath = s_filePath.substr( 0, pos++ );
        
        mkdir(subPath.c_str(), 0777);
    }
    mkdir(s_filePath.c_str(), 0777);
    
    std::string filePath = s_filePath + strFileName;
    log("File Path : %s", filePath.c_str());
    FILE *fp = fopen(filePath.c_str(), "wb");
    if ( !fp )
    {
        return 0;
    }
    
    CURL *_curl = curl_easy_init();
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, request->getUrl());
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, progresscallback);
    res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    
    fclose(fp);
    
    Director::getInstance()->getScheduler()->unscheduleSelector(schedule_selector(HBHttpClient::dispatchProgressCallbacks), HBHttpClient::getInstance());
    
    return res == 0 ? 0 : 1;
}
    
//Process POST Request
static int processPostTask(HBHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_POST, 1)
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process PUT Request
static int processPutTask(HBHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "PUT")
            && curl.setOption(CURLOPT_POSTFIELDS, request->getRequestData())
            && curl.setOption(CURLOPT_POSTFIELDSIZE, request->getRequestDataSize())
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}

//Process DELETE Request
static int processDeleteTask(HBHttpRequest *request, write_callback callback, void *stream, long *responseCode, write_callback headerCallback, void *headerStream, char *errorBuffer)
{
    CURLRaii curl;
    bool ok = curl.init(request, callback, stream, headerCallback, headerStream, errorBuffer)
            && curl.setOption(CURLOPT_CUSTOMREQUEST, "DELETE")
            && curl.setOption(CURLOPT_FOLLOWLOCATION, true)
            && curl.perform(responseCode);
    return ok ? 0 : 1;
}


// Process Response
static void processResponse(HBHttpResponse* response, char* errorBuffer)
{
    auto request = response->getHBHttpRequest();
    long responseCode = -1;
    int retValue = 0;

    // Process the request -> get response packet
    switch (request->getRequestType())
    {
    case HBHttpRequest::HBHttpRequestType::kHttpGet: // HTTP GET
        retValue = processGetTask(request,
            writeData, 
            response->getResponseData(), 
            &responseCode,
            writeHeaderData,
            response->getResponseHeader(),
            errorBuffer);
        break;
    case HBHttpRequest::HBHttpRequestType::kHttpGetFile:
            retValue = processGetFileTask(request,
                                          writeDataFile,
                                          progressFunc,
                                          response->getResponseData(),
                                          &responseCode,
                                          writeHeaderData,
                                          response->getResponseHeader());
            break;
            
    case HBHttpRequest::HBHttpRequestType::kHttpPost: // HTTP POST
        retValue = processPostTask(request,
            writeData, 
            response->getResponseData(), 
            &responseCode,
            writeHeaderData,
            response->getResponseHeader(),
            errorBuffer);
        break;

    case HBHttpRequest::HBHttpRequestType::kHttpPut:
        retValue = processPutTask(request,
            writeData,
            response->getResponseData(),
            &responseCode,
            writeHeaderData,
            response->getResponseHeader(),
            errorBuffer);
        break;

    case HBHttpRequest::HBHttpRequestType::kHttpDelete:
        retValue = processDeleteTask(request,
            writeData,
            response->getResponseData(),
            &responseCode,
            writeHeaderData,
            response->getResponseHeader(),
            errorBuffer);
        break;

    default:
        CCASSERT(true, "CCHBHttpClient: unkown request type, only GET and POSt are supported");
        break;
    }

    // write data to HBHttpResponse
    response->setResponseCode(responseCode);

    if (retValue != 0) 
    {
        response->setSucceed(false);
        response->setErrorBuffer(errorBuffer);
    }
    else
    {
        response->setSucceed(true);
    }
}

// HBHttpClient implementation
HBHttpClient* HBHttpClient::getInstance()
{
    if (s_pHBHttpClient == nullptr) {
        s_pHBHttpClient = new HBHttpClient();
    }
    
    return s_pHBHttpClient;
}

void HBHttpClient::destroyInstance()
{
    CC_SAFE_DELETE(s_pHBHttpClient);
}

void HBHttpClient::enableCookies(const char* cookieFile) {
    if (cookieFile) {
        s_cookieFilename = std::string(cookieFile);
    }
    else {
        s_cookieFilename = (FileUtils::getInstance()->getWritablePath() + "cookieFile.txt");
    }
}

HBHttpClient::HBHttpClient()
: _timeoutForConnect(30)
, _timeoutForRead(60)
{
}

HBHttpClient::~HBHttpClient()
{
    s_need_quit = true;
    
    if (s_requestQueue != nullptr) {
        s_SleepCondition.notify_one();
    }
    
    s_pHBHttpClient = nullptr;
}

//Lazy create semaphore & mutex & thread
bool HBHttpClient::lazyInitThreadSemphore()
{
    if (s_requestQueue != nullptr) {
        return true;
    } else {
        
        s_requestQueue = new Vector<HBHttpRequest*>();
        s_responseQueue = new Vector<HBHttpResponse*>();
        
		s_need_quit = false;
		
        auto t = std::thread(CC_CALLBACK_0(HBHttpClient::networkThread, this));
        t.detach();
    }
    
    return true;
}

//Add a get task to queue
void HBHttpClient::send(HBHttpRequest* request)
{    
    if (false == lazyInitThreadSemphore()) 
    {
        return;
    }
    
    if (!request)
    {
        return;
    }
        
    request->retain();
    
    if (nullptr != s_requestQueue) {
        s_requestQueueMutex.lock();
        s_requestQueue->pushBack(request);
        s_requestQueueMutex.unlock();
        
        // Notify thread start to work
        s_SleepCondition.notify_one();
    }
}

void HBHttpClient::sendImmediate(HBHttpRequest* request)
{
    if(!request)
    {
        return;
    }

    request->retain();
    auto t = std::thread(&HBHttpClient::networkThreadAlone, this, request);
    t.detach();
}

// Poll and notify main thread if responses exists in queue
void HBHttpClient::dispatchResponseCallbacks()
{
    // log("CCHBHttpClient::dispatchResponseCallbacks is running");
    //occurs when cocos thread fires but the network thread has already quited
    if (nullptr == s_responseQueue) {
        return;
    }
    HBHttpResponse* response = nullptr;
    
    s_responseQueueMutex.lock();

    if (!s_responseQueue->empty())
    {
        response = s_responseQueue->at(0);
        s_responseQueue->erase(0);
    }
    
    s_responseQueueMutex.unlock();
    
    if (response)
    {
        HBHttpRequest *request = response->getHBHttpRequest();
        const ccHttpRequestCallback& callback = request->getCallback();
        Ref* pTarget = request->getTarget();
        SEL_HBHttpResponse pSelector = request->getSelector();

        if (callback != nullptr)
        {
            callback(this, response);
        }
        else if (pTarget && pSelector)
        {
            (pTarget->*pSelector)(this, response);
        }
        
        response->release();
        // do not release in other thread
        request->release();
    }
}

#pragma mark - @승수 - 리소스 다운로드
void HBHttpClient::setFileSavePath(const char *filePath)
{
    s_filePath = filePath;
}

void HBHttpClient::dispatchProgressCallbacks(float delta)
{
    HBHttpRequest *s_requestTemp = s_request;
    if ( s_requestTemp != NULL )
    {
        Ref *pTarget = s_requestTemp->getTarget();
        SEL_CallFuncND pSelectorProgress = (SEL_CallFuncND) s_requestTemp->getProgressSelector();
        if (pTarget && pSelectorProgress)
        {
            (pTarget->*pSelectorProgress)((Node*)this, (void*)&s_nProgress);
        }
    }
}
    
}

NS_CC_END


