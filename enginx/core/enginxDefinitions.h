//
//  enginxDefinitions.h
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//
/**
 define all used enginx macros here
 */
#ifndef enginxDefinitions_h
#define enginxDefinitions_h

#include <iostream>
#include <string>
#include <vector>
#ifndef ENGINX_NAMESPACE
#define ENGINX_NAMESPACE enginx
#endif

#ifndef ENGINX_NAMESPACE_BEGIN
#define ENGINX_NAMESPACE_BEGIN namespace ENGINX_NAMESPACE {
#endif

#ifndef ENGINX_NAMESPACE_END
#define ENGINX_NAMESPACE_END }
#endif
//error code def
extern int const ENGINX_ERROR_BAD_PARAMETER;
//config fields def
extern const char* ENGINX_CONFIG_FIELD_SERVER_NAME;
extern const char* ENGINX_CONFIG_FIELD_SCHEMA;
extern const char* ENGINX_CONFIG_FIELD_PORT;
extern const char* ENGINX_CONFIG_FIELD_ACTION;

//default value def
extern const char* ENGINX_CONFIG_FIELD_DEF_DEFAULT_SERVER;
extern const char* ENGINX_CONFIG_FIELD_DEF_SERVER_WILD;


//internal vars def
extern const char* ENGINX_CONFIG_VAR_DEF_REQUEST_URI;
extern const char* ENGINX_CONFIG_VAR_DEF_HOST;
extern const char* ENGINX_CONFIG_VAR_DEF_SCHEME;
extern const char* ENGINX_CONFIG_VAR_DEF_QUERY_STRING;
extern const char* ENGINX_CONFIG_VAR_DEF_FRAGMENT;

//action instruction def
extern const char* ENGINX_CONFIG_INSTRUCTION_DECODE;
extern const char* ENGINX_CONFIG_INSTRUCTION_ENCODE;
extern const char* ENGINX_CONFIG_INSTRUCTION_PROXY_PASS;
extern const char* ENGINX_CONFIG_INSTRUCTION_RETURN;
extern const char* ENGINX_CONFIG_INSTRUCTION_TEMPORARILY;

ENGINX_NAMESPACE_BEGIN
class EnginxError {
public:
  std::string message;
  int code;
  EnginxError(const char*msg, int code);
  EnginxError() {};
};

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c) {
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
    
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

ENGINX_NAMESPACE_END

#endif /* enginxDefinitions_h */
