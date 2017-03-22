//
//  enginxInstance.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginxInstance.h"
#include <iostream>

ENGINX_NAMESPACE_BEGIN
using namespace rapidjson;
EnginxInstance::EnginxInstance() {}
EnginxInstance::~EnginxInstance() {}

void EnginxInstance::load(const char *config) {
  testing_config = config;
}

bool EnginxInstance::test(EnginxError& error) {
  testing_worker_config.Parse(testing_config.c_str());
  if (!testing_worker_config.IsArray()) {
    error = EnginxError("config must be an array", ENGINX_ERROR_BAD_PARAMETER);
    return false;
  }
  for (Value::ValueIterator itr = testing_worker_config.Begin(); itr != testing_worker_config.End(); ++itr) {
    if (!itr->IsObject()) {
      error = EnginxError("server config must be an json object", ENGINX_ERROR_BAD_PARAMETER);
      return false;
    }
    if (!itr->HasMember(ENGINX_CONFIG_FIELD_SERVER_NAME)) {
      error = EnginxError("missing required field server_name", ENGINX_ERROR_BAD_PARAMETER);
      return false;
    }
  }
  return true;
}

void EnginxInstance::reload() {
  Document::AllocatorType& allocator = current_worker_config.GetAllocator();
  current_worker_config.CopyFrom(testing_worker_config, allocator);
}

ENGINX_NAMESPACE_END
