//
//  enginxCocoaTouch.h
//  enginxCocoaTouch
//
//  Created by stephenw on 2017/3/20.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface enginxCocoaTouch : NSObject

+ (void)loadConfig:(NSString *)configString;
+ (NSString *)rewriteURLString:(NSString *)absoluteURLString;

@end
