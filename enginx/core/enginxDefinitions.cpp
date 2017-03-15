//
//  enginxDefinitions.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginxDefinitions.h"

int const ENGINX_ERROR_BAD_PARAMETER = 10400;
const char* ENGINX_CONFIG_FIELD_SERVER_NAME = "server_name";

ENGINX_NAMESPACE_BEGIN

EnginxError::EnginxError(const char*msg, int code) {
  message = msg;
  code = code;
}
ENGINX_NAMESPACE_END
