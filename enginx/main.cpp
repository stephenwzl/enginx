//
//  main.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include <iostream>
#include "core/enginx.h"
#include "core/vendor/http_parser/http_parser.h"
#include "core/enginxWorker.h"

using namespace enginx;
using namespace std;

int main(int argc, const char * argv[]) {
//  const char *json = "[{\"server_name\":\"enginx\"}]";
//  EnginxInstance::Instance().load(json);
//  bool tested = EnginxInstance::Instance().test();
//  cout<<tested<<endl;
  string json = "http://baidu.com:80/aaaa?restaurant=1#title";
//  enginx_url url;
//  http_parser_parse_url(json, strlen(json), 0, &url);
//  for (int i =0; i<UF_MAX; ++i) {
//    cout<<i<<"-----"<<"field offset:"<<url.field_data[i].off<<endl;
//    cout<<"field len:"<<url.field_data[i].len<<endl;
//  }
//  enginx_url& i = url;
  EnginxURL e_url = EnginxURL::EnginxURL(json);
  cout<<e_url.schema<<endl;

}
