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

EnginxLocation::EnginxLocation(rapidjson::Value& location_config, map<string, string>& vars, map<string, string> args, EnginxURL const url) {
  this->location_config.CopyFrom(location_config, this->location_config.GetAllocator());
  server_vars = std::map<string, string>(vars);
  query_args = std::map<string, string>(args);
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
    string after_url = server_vars[ENGINX_CONFIG_VAR_DEF_SCHEME] + "://";
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
        !server_vars[ENGINX_CONFIG_VAR_DEF_PATH].empty()) {
      std::smatch matches;
      std::regex mode;
      if (!RegexStringValid(parts[1], mode, false)) {
        return false;
      }
      std::regex_search(server_vars[ENGINX_CONFIG_VAR_DEF_PATH], matches, mode);
      computeInternalVars(matches);
      string template_str = parts[2];
      compileTemplates(template_str);
      current_url.path = template_str;
      server_vars[ENGINX_CONFIG_VAR_DEF_PATH] = current_url.path;
      server_vars[ENGINX_CONFIG_VAR_DEF_REQUEST_URI] = current_url.request_uri();
    } else if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_MATCH) == 0) {
      map<string, string>::iterator itr = server_vars.find(parts[1]);
      if (itr == server_vars.end()) {
        //can't find such server variable
        return false;
      }
      std::smatch matches;
      std::regex mode;
      if (!RegexStringValid(parts[2], mode, false)) {
        return false;
      }
      std::regex_search(server_vars[parts[1]], matches, mode);
      computeTempVars(matches);
    }
  }
  if (parts.size() == 2) {
    if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_PROXY_PASS) == 0) {
      string after_url = parts[1];
      after_url += ENGINX_CONFIG_VAR_DEF_REQUEST_URI;
      compileTemplates(after_url);
      rewrited_url = after_url;
      return false;
    }
    if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_RETURN) == 0) {
      string after_url = parts[1];
      compileTemplates(after_url);
      rewrited_url = after_url;
      return false;
    }
    if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_DECODE) == 0 ||
        parts[0].compare(ENGINX_CONFIG_INSTRUCTION_ENCODE) == 0) {
      execInternalVarsSubstitution(parts);
    }
    if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_PARSE) == 0) {
      map<string, string>::iterator itr = server_vars.find(parts[1]);
      if (itr == server_vars.end()) {
        //can't find such server variable
        return false;
      }
      std::string splitor = server_vars[parts[1]];
      vector<string> splits;
      SplitString(splitor, splits, "&");
      if (splits.size() == 0) return true;
      for (vector<string>::size_type i = 0; i != splits.size() ; ++i) {
        vector<string> key_value_pair;
        SplitString(splits[i], key_value_pair, "=");
        if (key_value_pair.size() != 2) continue;
        string key = "$#" + key_value_pair[0];
        temp_vars[key] = key_value_pair[1];
      }
    }
  }
  return true;
}

void EnginxLocation::execInternalVarsSubstitution(vector<string>& parts) {
  if (parts.size() != 2) {
    return;
  }
  map<string, string>::iterator itr;
  //sustitute internal variables
  for (itr = server_vars.begin(); itr != server_vars.end(); ++itr) {
    if (itr->first.compare(parts[1]) == 0) {
      if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_ENCODE) == 0) {
        itr->second = UrlEncode(itr->second);
      } else if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_DECODE) == 0) {
        itr->second = UrlDecode(itr->second);
      }
    }
  }
  //substitue query variables
  for (itr = query_args.begin(); itr != query_args.end(); ++itr) {
    if (itr->first.compare(parts[1]) == 0) {
      if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_ENCODE) == 0) {
        itr->second = UrlEncode(itr->second);
      } else if (parts[0].compare(ENGINX_CONFIG_INSTRUCTION_DECODE) == 0) {
        itr->second = UrlDecode(itr->second);
      }
    }
  }
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

void EnginxLocation::computeTempVars(std::smatch m) {
  if (!temp_vars.empty()) {
    temp_vars.clear();
  }
  unsigned int a = 0;
  for (auto x : m) {
    std::stringstream s;
    s<< "$#" << a;
    temp_vars[s.str()] = x.str();
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
  for (itr = query_args.begin(); itr != query_args.end(); ++itr) {
    string::size_type p = template_str.find(itr->first);
    if (p != string::npos) {
      template_str.replace(p, itr->first.length(), itr->second);
    }
  }
  for (itr = temp_vars.begin(); itr != temp_vars.end(); ++itr) {
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
    if (op.compare(ENGINX_CONFIG_OPERATOR_NORMAL_HEAD) == 0) {
      string rule = parts[1];
      auto res = std::mismatch(rule.begin(), rule.end(), request_path.begin());
      return res.first == rule.end();
    }
    if (op.compare(ENGINX_CONFIG_OPERATOR_REG_CASE_SENSITIVE) == 0) {
      std::regex mode;
      if (RegexStringValid(parts[1], mode, false)) {
        return std::regex_match(request_path, mode);
      }
    }
    if (op.compare(ENGINX_CONFIG_OPERATOR_REG_NO_CASE_SENSITIVE) == 0) {
      std::regex mode;
      if (RegexStringValid(parts[1], mode, true)) {
        return std::regex_match(request_path, mode);
      }
    }
  }
  return false;
}

void EnginxLocationDispatcher(EnginxURL const url, rapidjson::Value& locations, map<string, string>& server_vars, map<string, string>& query_args, string& rewrited_url) {
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
      EnginxLocation location_excutor(s, server_vars, query_args, url);
      if (!location_excutor.rewrited_url.empty()) {
        rewrited_url = location_excutor.rewrited_url;
      }
    }
  }
}

ENGINX_NAMESPACE_END
