/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
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

#ifndef __HTTP_RESPONSE__
#define __HTTP_RESPONSE__

#include "HBHttpRequest.h"

NS_CC_BEGIN

namespace hbnetwork {

/** 
 @brief defines the object which users will receive at onHttpCompleted(sender, HBHttpResponse) callback
 Please refer to samples/TestCpp/Classes/ExtensionTest/NetworkTest/HttpClientTest.cpp as a sample
 @since v2.0.2
 */
class HBHttpResponse : public cocos2d::Ref
{
public:
    /** Constructor, it's used by HttpClient internal, users don't need to create HBHttpResponse manually
     @param request the corresponding HBHttpRequest which leads to this response 
     */
    HBHttpResponse(HBHttpRequest* request)
    {
        _pHBHttpRequest = request;
        if (_pHBHttpRequest)
        {
            _pHBHttpRequest->retain();
        }
        
        _succeed = false;
        _responseData.clear();
        _errorBuffer.clear();
    }
    
    /** Destructor, it will be called in HttpClient internal,
     users don't need to desturct HBHttpResponse object manully 
     */
    virtual ~HBHttpResponse()
    {
        if (_pHBHttpRequest)
        {
            _pHBHttpRequest->release();
        }
    }
    
    /** Override autorelease method to prevent developers from calling it */
    cocos2d::Ref* autorelease(void)
    {
        CCASSERT(false, "HBHttpResponse is used between network thread and ui thread \
                        therefore, autorelease is forbidden here");
        return NULL;
    }
    
    // getters, will be called by users
    
    /** Get the corresponding HBHttpRequest object which leads to this response 
        There's no paired setter for it, coz it's already setted in class constructor
     */
    inline HBHttpRequest* getHBHttpRequest()
    {
        return _pHBHttpRequest;
    }
        
    /** To see if the http reqeust is returned successfully,
        Althrough users can judge if (http return code = 200), we want an easier way
        If this getter returns false, you can call getResponseCode and getErrorBuffer to find more details
     */
    inline bool isSucceed()
    {
        return _succeed;
    };
    
    /** Get the http response raw data */
    inline std::vector<char>* getResponseData()
    {
        return &_responseData;
    }
    
    /** get the Rawheader **/
    inline std::vector<char>* getResponseHeader()
    {
        return &_responseHeader;
    }

    /** Get the http response errorCode
     *  I know that you want to see http 200 :)
     */
    inline long getResponseCode()
    {
        return _responseCode;
    }

    /** Get the rror buffer which will tell you more about the reason why http request failed
     */
    inline const char* getErrorBuffer()
    {
        return _errorBuffer.c_str();
    }
    
    // setters, will be called by HttpClient
    // users should avoid invoking these methods
    
    
    /** Set if the http request is returned successfully,
     Althrough users can judge if (http code == 200), we want a easier way
     This setter is mainly used in HttpClient, users mustn't set it directly
     */
    inline void setSucceed(bool value)
    {
        _succeed = value;
    };
    
    
    /** Set the http response raw buffer, is used by HttpClient
     */
    inline void setResponseData(std::vector<char>* data)
    {
        _responseData = *data;
    }
    
    /** Set the http response Header raw buffer, is used by HttpClient
     */
    inline void setResponseHeader(std::vector<char>* data)
    {
        _responseHeader = *data;
    }
    
    
    /** Set the http response errorCode
     */
    inline void setResponseCode(long value)
    {
        _responseCode = value;
    }
    
    
    /** Set the error buffer which will tell you more the reason why http request failed
     */
    inline void setErrorBuffer(const char* value)
    {
        _errorBuffer.clear();
        _errorBuffer.assign(value);
    };
    
protected:
    bool initWithRequest(HBHttpRequest* request);
    
    // properties
    HBHttpRequest*        _pHBHttpRequest;  /// the corresponding HBHttpRequest pointer who leads to this response 
    bool                _succeed;       /// to indecate if the http reqeust is successful simply
    std::vector<char>   _responseData;  /// the returned raw data. You can also dump it as a string
    std::vector<char>   _responseHeader;  /// the returned raw header data. You can also dump it as a string
    long                _responseCode;    /// the status code returned from libcurl, e.g. 200, 404
    std::string         _errorBuffer;   /// if _responseCode != 200, please read _errorBuffer to find the reason 
    
};

}

NS_CC_END

#endif //__HTTP_RESPONSE_H__