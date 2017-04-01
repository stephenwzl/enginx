//
//  enginxCocoaTouch.m
//  enginxCocoaTouch
//
//  Created by stephenw on 2017/3/20.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#import "enginxCocoaTouch.h"
#include "enginx.h"

NSString *const EnginxCocoaErrorDomain = @"EnginxCocoaErrorDomain";

@implementation enginxCocoaTouch

+ (NSError *)loadConfig:(NSString *)configString {
  enginx::EnginxError error;
  bool loadSuccess = enginx::Enginx::load(configString.UTF8String, error);

  if (!loadSuccess) {
    return [EnginxError errorWithEnginxErrorCode:error.code errorMessage:[NSString stringWithUTF8String:error.message.c_str()]];
  }
  return nil;
}

+ (NSString *)rewriteURLString:(NSString *)absoluteURLString {
  std::string rewrited_url;
  enginx::Enginx::transfer(absoluteURLString.UTF8String, rewrited_url);
  return [NSString stringWithUTF8String:rewrited_url.c_str()];
}

@end

@implementation EnginxError

+ (instancetype)errorWithEnginxErrorCode:(NSInteger)code  errorMessage:(nonnull NSString *)message {
  return [super errorWithDomain:EnginxCocoaErrorDomain code:code userInfo:@{ @"message": message }];
}

- (NSString *)localizedDescription {
  return self.userInfo[@"message"];
}

- (NSString *)description {
  return [NSString stringWithFormat:@"EnginxError Domain=%@, code=%@, %@", EnginxCocoaErrorDomain, @(self.code).stringValue, self.localizedDescription];
}

- (NSString *)debugDescription {
  return [self description];
}

@end
