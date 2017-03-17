//
//  enginxLocationProcessor.cpp
//  enginx
//
//  Created by stephenw on 2017/3/17.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginxLocationProcessor.h"
#include <sstream>
ENGINX_NAMESPACE_BEGIN

using namespace rapidjson;
using namespace std;

EnginxLocation::EnginxLocation(rapidjson::Value& location_config,map<string, string>& vars, EnginxURL const url) {
  this->location_config.CopyFrom(location_config, this->location_config.GetAllocator());
  server_vars = std::map<string, string>(vars);
  current_url = url;
  if (!this->location_config.IsArray()) {
    return;
  }
  bool is_terminated = false;
  for (Value::ValueIterator itr = this->location_config.Begin(); itr < this->location_config.End(); ++itr) {
    if (itr->IsString()) {
      bool exec_next = resolveInstruction(itr->GetString());
      if (!exec_next) {
        is_terminated = true;
        break;
      }
    }
  }
  if (!is_terminated) {
    string after_url = server_vars[ENGINX_CONFIG_FIELD_SCHEMA] + "://";
    if (url.port != 80 && url.schema.compare("http") == 0) {
      std::stringstream s;
      s<<url.port;
      after_url += s.str();
    }
    after_url += server_vars[ENGINX_CONFIG_VAR_DEF_REQUEST_URI];
    rewrited_url = after_url;
  }
}

bool EnginxLocation::resolveInstruction(string instruction) {
  vector<string> parts;
  SplitString(instruction, parts, " ");
  if (parts.size() == 3) {
    if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_REWRITE) == 0 &&
        !server_vars[ENGINX_CONFIG_VAR_DEF_REQUEST_URI].empty()) {
      std::smatch matches;
      std::regex mode(parts[1]);
      std::regex_search(server_vars[ENGINX_CONFIG_VAR_DEF_REQUEST_URI], matches, mode);
      computeInternalVars(matches);
      string template_str = parts[2];
      compileTemplates(template_str);
      server_vars[ENGINX_CONFIG_VAR_DEF_REQUEST_URI] = template_str;
    }
  }
  if (parts.size() == 2) {
    if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_PROXY_PASS)) {
      string after_url = parts[1];
      compileTemplates(after_url);
      rewrited_url = after_url;
      return false;
    }
    if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_RETURN)) {
      string after_url = parts[1];
      compileTemplates(after_url);
      rewrited_url = after_url;
      return false;
    }
  }
  return true;
}

void EnginxLocation::computeInternalVars(std::smatch m) {
  if (!internal_vars.empty()) {
    internal_vars.clear();
  }
  unsigned int a = 0;
  for (auto x : m) {
    std::stringstream s;
    s<< "$" << a;
    internal_vars[s.str()] = x.str();
    a++;
  }
}

void EnginxLocation::compileTemplates(string& template_str) {
  if (template_str.empty()) {
    return;
  }
  map<string, string>::iterator itr;
  for (itr = server_vars.begin(); itr != server_vars.end(); ++itr) {
    string::size_type p = template_str.find(itr->first);
    if (p != string::npos) {
      template_str.replace(p, itr->first.length(), itr->second);
    }
  }
  for (itr = internal_vars.begin(); itr != internal_vars.end(); ++itr) {
    string::size_type p = template_str.find(itr->first);
    if (p != string::npos) {
      template_str.replace(p, itr->first.length(), itr->second);
    }
  }
}

bool sortLocation(string& v1, string& v2) {
  vector<string> parts_v1;
  vector<string> parts_v2;
  SplitString(v1, parts_v1, " ");
  SplitString(v2, parts_v2, " ");
  if (parts_v1.size() > 2 || parts_v1.size() == 0) return false;
  if (parts_v2.size() > 2 || parts_v2.size() == 0) return true;
  string operator_v1 = parts_v1[0];
  string operator_v2 = parts_v2[0];
  if (parts_v1.size() == 1) {
//    if (operator_v1.compare(ENGINX_CONFIG_OPERATOR_WILD_COMPARE) == 0) return false;
//    if (parts_v2.size() == 1 && operator_v2.compare(ENGINX_CONFIG_OPERATOR_WILD_COMPARE))
//      return false;
    return false;
  }
  if (parts_v1.size() == 2) {
    if (operator_v1.compare(ENGINX_CONFIG_OPERATOR_EQUAL) == 0) return true;
    
    if (parts_v2.size() == 1) return true;
//      return operator_v2.compare(ENGINX_CONFIG_OPERATOR_WILD_COMPARE) == 0;

    if (operator_v1.compare(ENGINX_CONFIG_OPERATOR_NORMAL_HEAD) == 0) {
        return operator_v2.compare(ENGINX_CONFIG_OPERATOR_EQUAL) != 0;
    }
    if (operator_v1.compare(ENGINX_CONFIG_OPERATOR_REG_CASE_SENSITIVE) == 0) {
      return operator_v2.compare(ENGINX_CONFIG_OPERATOR_NORMAL_HEAD) !=0 &&
      operator_v2.compare(ENGINX_CONFIG_OPERATOR_EQUAL) !=0;
    }
    if (operator_v1.compare(ENGINX_CONFIG_OPERATOR_REG_NO_CASE_SENSITIVE) == 0) return false;
  }
  return false;
}

bool is_location_matching(string operation, string request_path, bool* search_next) {
  vector<string> parts;
  *search_next = false;
  SplitString(operation, parts, " ");
  if (parts.size() == 0 || parts.size() > 2) return false;
  string op = parts[0];
  if (parts.size() == 1) {
    if (op.compare(ENGINX_CONFIG_OPERATOR_WILD_COMPARE) == 0) {
      *search_next = true;
      return true;
    }
  }
  if (parts.size() == 2) {
    if (op.compare(ENGINX_CONFIG_OPERATOR_EQUAL) == 0) {
      *search_next = false;
      return parts[1].compare(request_path) == 0;
    }
    if (op.compare(ENGINX_CONFIG_OPERATOR_NORMAL_HEAD)) {
      auto res = std::mismatch(op.begin(), op.end(), request_path.begin());
      return res.first == op.end();
    }
    if (op.compare(ENGINX_CONFIG_OPERATOR_REG_CASE_SENSITIVE)) {
      std::regex mode(parts[1]);
      return std::regex_match(request_path, mode);
    }
    if (op.compare(ENGINX_CONFIG_OPERATOR_REG_NO_CASE_SENSITIVE)) {
      std::regex mode(parts[1], std::regex::icase);
      return std::regex_match(request_path, mode);
    }
  }
  return false;
}

void EnginxLocationDispatcher(EnginxURL const url, rapidjson::Value& locations, map<string, string>& server_vars, string& rewrited_url) {
  if (!locations.IsObject()) {
    return;
  }
  vector<string> location_keys;
  for (Value::ConstMemberIterator itr = locations.MemberBegin(); itr < locations.MemberEnd(); ++itr) {
//    if (!itr->IsString()) {
//      continue;
//    }
    if (strlen(itr->name.GetString()) == 0) {
      continue;
    }
    location_keys.push_back(itr->name.GetString());
  }
  if (location_keys.size() == 0) return;
  std::sort(location_keys.begin(), location_keys.end(), sortLocation);
  string executor;
  bool search_next;
  for (vector<string>::size_type i = 0; i != location_keys.size(); ++i) {
    if(is_location_matching(location_keys[i], url.path, &search_next)) {
      executor = location_keys[i];
      if (search_next) {
        continue;
      } else {
        break;
      }
    }
  }
  if (!executor.empty()) {
    Value& s = locations[executor.c_str()];
    if (!s.IsNull() && s.IsArray()) {
      EnginxLocation location_excutor(s, server_vars, url);
      if (!location_excutor.rewrited_url.empty()) {
        rewrited_url = location_excutor.rewrited_url;
      }
    }
  }
}

ENGINX_NAMESPACE_END
