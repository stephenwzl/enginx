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
#include "enginxWorker.h"

ENGINX_NAMESPACE_BEGIN
class Enginx {
public:
  static bool load(const char* config, EnginxError& error) {
    EnginxInstance::Instance().load(config);
    if (EnginxInstance::Instance().test(error)) {
      EnginxInstance::Instance().reload();
      return true;
    } else {
      return false;
    }
  }
  
  /**
   rewrite an absolute url to another url

   @param absolute_url the absolute url includes scheme, host, path, fragment querystring
   */
  static void  transfer(const char* absolute_url, std::string& rewrited_url) {
    EnginxWorker work(absolute_url);
    rewrited_url = work.rewirted_url;
  }
};
ENGINX_NAMESPACE_END

#endif /* enginx_h */
