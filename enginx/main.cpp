//
//  main.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include <iostream>
#include "core/enginx.h"

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
  Enginx::transfer("http://baidu.com/api/auth", rewrited);
  cout<<"test rewrite:"<<endl;
  cout<< rewrited<<endl;
}

int main(int argc, const char * argv[]) {
  string json = "[\
  {\
    \"server_name\":\"baidu.com\", \
    \"action\":[\
      \"decode $query_string\"\
      ],\
    \"location\":{\
        \"/\":[\
          \"return http://stephenw.cc\"\
        ],\
        \"= /path\":[\
          \"proxy_pass http://google.com\"    \
        ],\
        \"^~ /api\":[\
          \"rewrite ^/api/(.*)$ /$1\",\
          \"proxy_pass http://stephenw.cc\"\
        ]\
      }\
    }\
  ]";
  EnginxError error;
  Enginx::load(json.c_str(), error);
  
  test_location_equal();
  test_rewrite();
}
