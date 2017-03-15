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
      error.code = ENGINX_ERROR_BAD_PARAMETER;
      error.message = "enginx config test failed";
      return false;
    }
  }
};
ENGINX_NAMESPACE_END

#endif /* enginx_h */
