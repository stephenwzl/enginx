//
//  enginxInstance.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginxInstance.h"
#include <iostream>
#include <vector>

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
  bool success = testFormat(error);
  if (!success) return success;
  
  return true;
}

bool EnginxInstance::testFormat(enginx::EnginxError &error) {
  for (Value::ValueIterator itr = testing_worker_config.Begin(); itr != testing_worker_config.End(); ++itr) {
    Value& s = *itr;
    if (s[ENGINX_CONFIG_FIELD_SERVER_NAME].IsString()) {
      std::string s_name = s[ENGINX_CONFIG_FIELD_SERVER_NAME].GetString();
      if (s_name.empty()) {
        error = EnginxError("server_name field can't be empty", ENGINX_ERROR_BAD_PARAMETER);
        return false;
      }
    } else {
      error = EnginxError("server_name field type mismatch, expected string", ENGINX_ERROR_BAD_PARAMETER);
      return false;
    }
    if (s.HasMember(ENGINX_CONFIG_FIELD_ACTION)) {
      Value& action = s[ENGINX_CONFIG_FIELD_ACTION];
      if (!action.IsArray()) {
        error = EnginxError("action field type mismatch, expected array", ENGINX_ERROR_BAD_PARAMETER);
        return false;
      }
      if (!testAction(error, action)) return false;
    }
    if (s.HasMember(ENGINX_CONFIG_FIELD_LOCATION)) {
      Value& location = s[ENGINX_CONFIG_FIELD_LOCATION];
      if (!location.IsObject()) {
        error = EnginxError("location field type error, expected object with keys", ENGINX_ERROR_BAD_PARAMETER);
        return false;
      }
      if (!testLocations(error, location)) return false;
    }
    
  }
  return true;
}

bool EnginxInstance::testAction(enginx::EnginxError &error, rapidjson::Value &action) {
  for (Value::ValueIterator itr = action.Begin(); itr != action.End(); ++itr) {
    if (!itr->IsString()) {
      error = EnginxError("action's instruction type must be string", ENGINX_ERROR_BAD_PARAMETER);
      return false;
    }
    if (!testInstruction(error, itr->GetString(), false)) {
      return false;
    }
  }
  return true;
}

bool EnginxInstance::testLocations(enginx::EnginxError &error, rapidjson::Value &locations) {
  for (Value::ConstMemberIterator itr = locations.MemberBegin(); itr != locations.MemberEnd(); ++itr) {
    if (!itr->value.IsArray()) {
      error = EnginxError("location's instruction field type error, expected array", ENGINX_ERROR_BAD_PARAMETER);
      return false;
    }
    for (Value::ConstValueIterator v_itr = itr->value.Begin(); v_itr != itr->value.End(); ++v_itr) {
      if (!v_itr->IsString()) {
        error = EnginxError("locations's instruction type must be string", ENGINX_ERROR_BAD_PARAMETER);
        return false;
      }
      if (!testInstruction(error, v_itr->GetString(), true)) return false;
    }
  }
  return true;
}

bool EnginxInstance::testInstruction(enginx::EnginxError &error, std::string instruction, bool isInLocationField) {
  std::vector<std::string> parts;
  SplitString(instruction, parts, " ");
  if (parts.size() < 2) {
    std::string e_msg = "instruction:'"+instruction+"' format error";
    error = EnginxError(e_msg.c_str(), ENGINX_ERROR_BAD_PARAMETER);
    return false;
  }
  std::string ins = parts[0];
  if ((ins.compare(ENGINX_CONFIG_INSTRUCTION_REWRITE) == 0 ||
      ins.compare(ENGINX_CONFIG_INSTRUCTION_PROXY_PASS) == 0 ||
       ins.compare(ENGINX_CONFIG_INSTRUCTION_MATCH) == 0 ||
       ins.compare(ENGINX_CONFIG_INSTRUCTION_PARSE) == 0) && !isInLocationField) {
    error = EnginxError("found instructions not supported in action area", ENGINX_ERROR_BAD_PARAMETER);
    return false;
  }
  if (parts.size() == 2) {
    if (ins.compare(ENGINX_CONFIG_INSTRUCTION_REWRITE) == 0) {
      error = EnginxError("rewrite instruction must receieve 2 parameters", ENGINX_ERROR_BAD_PARAMETER);
      return false;
    }
  }
  if (parts.size() == 3) {
    if (ins.compare(ENGINX_CONFIG_INSTRUCTION_REWRITE) == 0) {
      if (parts[1].empty() || parts[2].empty()) {
        error = EnginxError("rewrite regex or string template can't be empty", ENGINX_ERROR_BAD_PARAMETER);
        return false;
      }
      std::regex mode;
      if (!RegexStringValid(parts[1], mode, false)) {
        error = EnginxError("rewrite regex not valid", ENGINX_ERROR_BAD_PARAMETER);
        return false;
      }
    }
  }
  return true;
}

void EnginxInstance::reload() {
  Document::AllocatorType& allocator = current_worker_config.GetAllocator();
  current_worker_config.CopyFrom(testing_worker_config, allocator);
}

ENGINX_NAMESPACE_END
