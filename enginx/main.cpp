//
//  main.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include <iostream>
#include "time.h"
#include "core/enginx.h"
#include <regex>

using namespace enginx;
using namespace std;

void test_location_equal() {
  string rewrited;
  Enginx::transfer("http://baidu.com/path?url=%e5%93%88%e5%93%88", rewrited);
  cout<<"test location equal:"<<endl;
  cout<< rewrited<<endl;
}

void test_rewrite() {
  string rewrited;
  Enginx::transfer("http://baidu.com/api/auth/123", rewrited);
  cout<<"test rewrite:"<<endl;
  cout<< rewrited<<endl;
}

void test_location_regex() {
  string rewrited;
  Enginx::transfer("http://baidu.com/56789.jpg", rewrited);
  cout<<"test location regex:"<<endl;
  cout<< rewrited<<endl;
}

void test_time() {
  clock_t start, finish;
  start = clock();
  for (int i = 0; i < 10000; ++i) {
    string rewrited;
    Enginx::transfer("http://baidu.com/api/auth/123", rewrited);
  }
  finish = clock();
  cout << "time consumed:" << (double)(finish - start)/CLOCKS_PER_SEC <<endl;
  
}

int main(int argc, const char * argv[]) {
  string json = "[\
  {\
    \"server_name\":\"baidu.com\", \
    \"location\":{\
        \"/\":[\
          \"return http://stephenw.cc\"\
        ],\
        \"= /path\":[\
          \"decode $arg_url\",\
          \"return http://google.com?token=$arg_url\"    \
        ],\
        \"^~ /api\":[\
          \"rewrite ^/api/(.*)$ /$1\",\
          \"return http://stephenw.cc$path\"\
        ],\
        \"~* .*(gif|jpg|jpeg)$\":[\
          \"proxy_pass http://ele.me\"\
        ]\
      }\
    }\
  ]";
  EnginxError error;
  Enginx::load(json.c_str(), error);
  if (error.code) {
    cout << error.message << endl;
  } else {
    test_location_equal();
    test_rewrite();
    //test_location_regex();
//    test_time();
  }
  std::regex mode;
  bool isvalid = RegexStringValid("*", mode, false);
  cout<<isvalid<<endl;
}
