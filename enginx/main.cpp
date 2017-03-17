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

int main(int argc, const char * argv[]) {
  string json = "[\
  {\
    \"server_name\":\"baidu.com\", \
    \"action\":[\
      \"decode $query_string\"\
      ],\
    \"location\":{\
        \"/\":[\
          \"proxy_pass https://stephenw.cc$request_uri\"\
        ]\
      }\
    }\
  ]";
  EnginxError error;
  bool tested = Enginx::load(json.c_str(), error);
  cout << tested << endl;
  cout << Enginx::transfer("http://baidu.com/path?url=%e5%93%88%e5%93%88") << endl;
}
