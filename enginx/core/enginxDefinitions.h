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

#ifndef ENGINX_NAMESPACE
#define ENGINX_NAMESPACE enginx
#endif

#ifndef ENGINX_NAMESPACE_BEGIN
#define ENGINX_NAMESPACE_BEGIN namespace ENGINX_NAMESPACE {
#endif

#ifndef ENGINX_NAMESPACE_END
#define ENGINX_NAMESPACE_END }
#endif
extern int const ENGINX_ERROR_BAD_PARAMETER;
extern const char* ENGINX_CONFIG_FIELD_SERVER_NAME;

ENGINX_NAMESPACE_BEGIN
class EnginxError {
public:
  std::string message;
  int code;
  EnginxError(const char*msg, int code);
};
ENGINX_NAMESPACE_END

#endif /* enginxDefinitions_h */
