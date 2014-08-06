//
//  FrameworkString.h
//  Highbrow_Framework
//
//  Created by 김수영 on 14. 6. 4..
//  Copyright (c) 2014년 김수영. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface FrameworkString : NSObject <NSXMLParserDelegate>{
    
    NSMutableDictionary *stringDictnary;
}
- (void)highbrowGameInfos:(NSString *)gameName :(NSString *)languageCode;

+(id)sharedManager;

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qualifiedName attributes:(NSDictionary *)attributeDict;
- (NSString*)GetString:(NSString*)key;


@end

