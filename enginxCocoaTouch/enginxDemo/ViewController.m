//
//  ViewController.m
//  enginxDemo
//
//  Created by stephenw on 2017/3/20.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#import "ViewController.h"
#import "enginxCocoaTouch.h"
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  NSString *json = @"[\
  {\
  \"server_name\":\"baidu.com\", \
  \"action\":[\
  \"decode $query_string\",\
  \"proxy_pass http://baidu.com\"\
  ],\
  \"location\":{\
  \"/\":[\
  \"proxy_pass https://stephenw.cc$request_uri\"\
  ]\
  }\
  }\
  ]";
  NSError *error = [enginxCocoaTouch loadConfig:json];
  if (error) {
    NSLog(@"%@", error);
  }
  NSString *url = @"http://baidu.com/path?url=%e5%93%88%e5%93%88";
  NSLog(@"%@", [enginxCocoaTouch rewriteURLString:url]);
}


- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
  // Dispose of any resources that can be recreated.
}


@end
