//
//  enginxCocoaTouch.m
//  enginxCocoaTouch
//
//  Created by stephenw on 2017/3/20.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#import "enginxCocoaTouch.h"
#include "enginx.h"

@implementation enginxCocoaTouch

+ (void)loadConfig:(NSString *)configString {
  enginx::EnginxError error;
  enginx::Enginx::load(configString.UTF8String, error);
}

+ (NSString *)rewriteURLString:(NSString *)absoluteURLString {
  std::string rewrited_url;
  enginx::Enginx::transfer(absoluteURLString.UTF8String, rewrited_url);
  return [NSString stringWithUTF8String:rewrited_url.c_str()];
}

@end
