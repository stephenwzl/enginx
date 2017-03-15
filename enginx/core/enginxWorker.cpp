//
//  enginxWorker.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginxWorker.h"

ENGINX_NAMESPACE_BEGIN

bool FieldValid(int offset, int len, size_t url_len) {
  if (offset > url_len) {
    return false;
  }
  if (len == 0) {
    return false;
  }
  return true;
}

EnginxURL::EnginxURL(string const absolute_url_string) {
  port = 80;//default port
  enginx_url url;
  http_parser_parse_url(absolute_url_string.c_str(), absolute_url_string.length(), 0, &url);
  absolute_url = absolute_url_string;
  size_t len = absolute_url_string.length();
  for (int i = UF_SCHEMA; i < UF_MAX; ++i) {
    if (!FieldValid(url.field_data[i].off,
                   url.field_data[i].len,len)) {
      continue;
    }
    switch (i) {
      case UF_SCHEMA:
        schema = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_HOST:
        host = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_PATH:
        path = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_PORT:
      {
        string portstr = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        if (portstr.length() == 0) {
          if (host.compare("http") == 0) {
            port = 80;
          } else if (host.compare("https")) {
            port = 443;
          }
        } else {
          port = atoi(portstr.c_str());
        }
        break;
      }
      case UF_QUERY:
      
        break;
    }
  }
}

EnginxURL::~EnginxURL() {}


EnginxWorker::EnginxWorker(char* const url) {
  http_parser_parse_url(url, strlen(url), 0, &uri);
  
}

ENGINX_NAMESPACE_END
