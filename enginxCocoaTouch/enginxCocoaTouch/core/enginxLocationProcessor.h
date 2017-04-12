//
//  enginxLocationProcessor.h
//  enginx
//
//  Created by stephenw on 2017/3/17.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#ifndef enginxLocationProcessor_h
#define enginxLocationProcessor_h
#include "enginxDefinitions.h"
#include "enginxWorker.h"
#include <map>

ENGINX_NAMESPACE_BEGIN
using namespace std;

class EnginxLocation {
public:
  EnginxLocation(rapidjson::Value& location_config, map<string, string>& vars, map<string, string> args, EnginxURL const url);
  string rewrited_url;
  EnginxURL current_url;
private:
  rapidjson::Document location_config;
  std::map<string, string> internal_vars;
  std::map<string, string> temp_vars;
  std::map<string, string> server_vars;
  std::map<string, string> query_args;
  std::string optional_mode;
  bool resolveInstruction(string instruction);
  void computeInternalVars(vector<string>& m);
  void computeTempVars(vector<string>& m);
  void compileTemplates(string& template_str);
  bool compileOptionalSections(string& template_str);
  bool substitueStrVars(string& template_str);
  void execInternalVarsCoding(vector<string>& parts);
};

void EnginxLocationDispatcher(EnginxURL const url, rapidjson::Value& locations, map<string, string>& server_vars, map<string, string>& query_args, string& rewrited_url);

ENGINX_NAMESPACE_END

#endif /* enginxLocationProcessor_h */
