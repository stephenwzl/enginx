//
//  enginx.h
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#ifndef enginx_h
#define enginx_h

#include "enginxDefinitions.h"
#include "enginxInstance.h"

ENGINX_NAMESPACE_BEGIN
class Enginx {
public:
  static bool load(char* const config, EnginxError& error) {
    EnginxInstance::Instance().load(config);
    if (EnginxInstance::Instance().test()) {
      EnginxInstance::Instance().reload();
      return true;
    } else {
      error = EnginxError("config test failed", ENGINX_ERROR_BAD_PARAMETER);
      return false;
    }
  }
  
  /**
   rewrite an absolute url to another url

   @param absolute_url the absolute url includes scheme, host, path, fragment querystring
   @return rewrited url
   */
  static char* const transfer(char* const absolute_url) {
    std::string a = "";
    return (char *)a.c_str();
  }
};
ENGINX_NAMESPACE_END

#endif /* enginx_h */
