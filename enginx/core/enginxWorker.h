//
//  enginxWorker.h
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#ifndef enginxWorker_h
#define enginxWorker_h

#include "enginxInstance.h"
#include "vendor/http_parser/http_parser.h"
#include <vector>
ENGINX_NAMESPACE_BEGIN

typedef http_parser_url enginx_url;
using namespace std;

class EnginxURL {
public:
  string schema;
  unsigned int port;
  string host;
  string path;
  string querystring;
  string fragment;
  string userinfo;
  
  string absolute_url;
  EnginxURL() {};
  bool isValid();
  string request_uri();
  EnginxURL(string const absolute_url_string);
  ~EnginxURL();
};

class EnginxWorker {
public:
  EnginxWorker(string absolute_url_string);
  string rewirted_url;
private:
  EnginxURL current_url;
  rapidjson::Document worker_config;
  rapidjson::Document response_servers;
};

ENGINX_NAMESPACE_END

#endif /* enginxWorker_h */
