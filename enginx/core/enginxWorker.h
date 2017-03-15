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
  EnginxURL(string const absolute_url_string);
  ~EnginxURL();
};

class EnginxWorker {
public:
  EnginxWorker(char* const url);
private:
  enginx_url uri;
};

ENGINX_NAMESPACE_END

#endif /* enginxWorker_h */
