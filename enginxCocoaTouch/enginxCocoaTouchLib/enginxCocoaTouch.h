//
//  enginxCocoaTouch.h
//  enginxCocoaTouch
//
//  Created by stephenw on 2017/3/20.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface enginxCocoaTouch : NSObject


/**
 load config string returns error

 @param configString json config format
 @return error, nullable
 */
+ (__kindof NSError * _Nullable)loadConfig:(NSString *)configString;


/**
 give a url string to rewrite

 @param absoluteURLString the absolute url string
 @return rewrited url string, the same if rewrite no effects
 */
+ (NSString *)rewriteURLString:(NSString *)absoluteURLString;

@end

@interface EnginxError : NSError

+ (instancetype)errorWithEnginxErrorCode:(NSInteger)code errorMessage:(NSString *)message;

@end

NS_ASSUME_NONNULL_END
