//
//  HBNetworkFileDownload.h
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 25..
//
//

#ifndef cocos2d_libs_HBNetworkFileIOS_h
#define cocos2d_libs_HBNetworkFileIOS_h

#include "HBNetworkFile.h"
#import "AFNetworking.h"

@interface HBNetworkFileIOS_objc : NSObject <NSURLSessionDownloadDelegate>
{
    void* netwokrFile_;
}

@property (nonatomic, assign) void* netwokrFile;
@property (nonatomic, strong) NSString* strPath;
@property (nonatomic, strong) NSURLSession *session;
@property (nonatomic, strong) NSURLSessionDownloadTask *downloadTask;

-(id) initWithNetwork : (void*)networkFile;
-(void) download : (NSString *)strUrl strPath:(NSString *)strPath;

@end

NS_CC_BEGIN

class CC_DLL HBNetworkFileIOS : public HBNetworkFile
{
public:
    
    HBNetworkFileIOS(void);
    virtual ~HBNetworkFileIOS(void);

public:
    /** 메모리 용량 */
    virtual long long getMemorySize() override;
    
    /** 다운로드 */
    virtual void downloadTask(std::string strUrl, std::string strPath) override;
    
    
private:
protected:
    HBNetworkFileIOS_objc* _networkControl;

};

NS_CC_END

#endif
