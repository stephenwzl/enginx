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
const char* ENGINX_CONFIG_FIELD_SCHEMA = "scheme";
const char* ENGINX_CONFIG_FIELD_PORT = "listen";
const char* ENGINX_CONFIG_FIELD_ACTION = "action";

const char* ENGINX_CONFIG_FIELD_DEF_DEFAULT_SERVER = "default_server";
const char* ENGINX_CONFIG_FIELD_DEF_SERVER_WILD = "*";

const char* ENGINX_CONFIG_VAR_DEF_REQUEST_URI = "$request_uri";
const char* ENGINX_CONFIG_VAR_DEF_HOST = "$host";
const char* ENGINX_CONFIG_VAR_DEF_SCHEME = "$scheme";
const char* ENGINX_CONFIG_VAR_DEF_QUERY_STRING = "$query_string";
const char* ENGINX_CONFIG_VAR_DEF_FRAGMENT = "$fragment";

const char* ENGINX_CONFIG_INSTRUCTION_DECODE = "decode";
const char* ENGINX_CONFIG_INSTRUCTION_ENCODE = "encode";
const char* ENGINX_CONFIG_INSTRUCTION_PROXY_PASS = "proxy_pass";
const char* ENGINX_CONFIG_INSTRUCTION_RETURN = "return";
const char* ENGINX_CONFIG_INSTRUCTION_TEMPORARILY = "temporarily";

ENGINX_NAMESPACE_BEGIN

EnginxError::EnginxError(const char*msg, int code) {
  message = msg;
  code = code;
}
ENGINX_NAMESPACE_END
