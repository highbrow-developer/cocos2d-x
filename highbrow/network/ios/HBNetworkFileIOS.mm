//
//  HBNetworkFileDownload.h
//  cocos2d_libs
//
//  Created by 이명열 on 2014. 8. 25..
//
//

#include "HBNetworkFileIOS.h"

@implementation HBNetworkFileIOS_objc

@synthesize netwokrFile = netwokrFile_;

- (void)dealloc
{
    self.netwokrFile = nil;
    [super dealloc];
}

-(id) initWithNetwork : (void*)networkFile
{
    self = [super init];
    if ( self )
    {
        NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration backgroundSessionConfiguration:@"com.highbrow.mytest01"];
        self.session = [NSURLSession sessionWithConfiguration:configuration delegate:self delegateQueue:nil];
        
        self.netwokrFile = networkFile;
    }
    
    return self;
}


-(void) download : (NSString *)strUrl strPath:(NSString *)strPath
{
    NSLog(@"다운 %@", strUrl);
    NSLog(@"주소 %@", strPath);
    self.strPath = strPath;
    
    NSURL *URL = [NSURL URLWithString:strUrl];
    NSURLRequest *request = [NSURLRequest requestWithURL:URL];

    self.downloadTask = [self.session downloadTaskWithRequest:request];
    [self.downloadTask resume];
}

-(void) URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didFinishDownloadingToURL:(NSURL *)location
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    NSURL *documentsDirectory = [NSURL fileURLWithPath:self.strPath];
    
    NSURL *originalURL = [[downloadTask originalRequest] URL];
    NSURL *destinationURL = [documentsDirectory URLByAppendingPathComponent:[originalURL lastPathComponent]];
    NSError *errorCopy;
    
    [fileManager removeItemAtURL:destinationURL error:NULL];
    BOOL success = [fileManager copyItemAtURL:location toURL:destinationURL error:&errorCopy];
    if ( success )
    {
        NSLog(@"the copy %@", [originalURL path]);
    }
    else
    {
        NSLog(@"Error during the copy: %@", [errorCopy localizedDescription]);
    }
}

-(void) URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didResumeAtOffset:(int64_t)fileOffset expectedTotalBytes:(int64_t)expectedTotalBytes
{
    
}

-(void) URLSession:(NSURLSession *)session downloadTask:(NSURLSessionDownloadTask *)downloadTask didWriteData:(int64_t)bytesWritten totalBytesWritten:(int64_t)totalBytesWritten totalBytesExpectedToWrite:(int64_t)totalBytesExpectedToWrite
{
    if (downloadTask == self.downloadTask)
    {
        double progress = (double)totalBytesWritten / (double)totalBytesExpectedToWrite;
        if ( self.netwokrFile != nil )
        {
            ((cocos2d::HBNetworkFileIOS*)self.netwokrFile)->downloadNow(progress);
        }
    }
}

-(void) URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error
{
    if (error != nil)
    {
        NSLog(@"Download completed with error: %@", [error localizedDescription]);
        if ( self.netwokrFile != nil )
        {
            ((cocos2d::HBNetworkFileIOS*)self.netwokrFile)->downloadError();
        }
    }
    else
    {
        NSLog(@"Download finished successfully.");
        if ( self.netwokrFile != nil )
        {
            ((cocos2d::HBNetworkFileIOS*)self.netwokrFile)->downloadEnd();
        }
    }
    
    self.downloadTask = nil;
}

@end

NS_CC_BEGIN

HBNetworkFileIOS::HBNetworkFileIOS()
{
    _networkControl = [[HBNetworkFileIOS_objc alloc] initWithNetwork:this];
}

HBNetworkFileIOS::~HBNetworkFileIOS()
{
    [_networkControl release];
}

HBNetworkFile* HBNetworkFile::getInstance()
{
    if ( s_pHBNetworkFile == nullptr )
    {
        s_pHBNetworkFile = new HBNetworkFileIOS();
        s_pHBNetworkFile->init();
    }
    
    return s_pHBNetworkFile;
}

void HBNetworkFileIOS::downloadTask(std::string strUrl, std::string strPath)
{
    NSString *pStrUrl = [NSString stringWithCString:strUrl.c_str() encoding:NSUTF8StringEncoding];
    NSString *pStrPath = [NSString stringWithCString:strPath.c_str() encoding:NSUTF8StringEncoding];
    [_networkControl download:pStrUrl strPath:pStrPath];
}

long long HBNetworkFileIOS::getMemorySize()
{
    long long nResult = 0;
    
    __autoreleasing NSError *error = nil;
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSDictionary *dictionary = [[NSFileManager defaultManager] attributesOfFileSystemForPath:[paths lastObject] error: &error];
    
    if (dictionary)
    {
        NSNumber *freeFileSystemSizeInBytes = [dictionary objectForKey:NSFileSystemFreeSize];
        nResult = [freeFileSystemSizeInBytes longLongValue];
    }
    
    return nResult;
}

NS_CC_END

