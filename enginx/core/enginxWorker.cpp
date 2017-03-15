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

EnginxURL::EnginxURL(enginx_url url, char* const absolute_url_string) {
  absolute_url = (char*)malloc(sizeof(absolute_url_string));
  strcpy(absolute_url, absolute_url_string);
  size_t len = sizeof(absolute_url);
  for (int i = UF_SCHEMA; i < UF_MAX; ++i) {
    if (!FieldValid(url.field_data[i].off,
                   url.field_data[i].len,len)) {
      continue;
    }
    switch (i) {
      case UF_SCHEMA:
        schema = (char*)malloc(url.field_data[i].len *sizeof(char));
        
        break;
        
      default:
        break;
    }
  }
}

EnginxURL::~EnginxURL() {
  free(absolute_url);
  
}


EnginxWorker::EnginxWorker(char* const url) {
  http_parser_parse_url(url, strlen(url), 0, &uri);
  
}

ENGINX_NAMESPACE_END
