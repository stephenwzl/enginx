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

class EnginxURL {
public:
  char* schema;
  unsigned int port;
  char* host;
  char* path;
  char* querystring;
  char* fragment;
  char* userinfo;
  
  char* absolute_url;
  EnginxURL(enginx_url url, char* const absolute_url_string);
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
