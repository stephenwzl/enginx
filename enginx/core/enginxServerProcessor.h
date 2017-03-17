//
//  enginxServerProcessor.h
//  enginx
//
//  Created by stephenw on 2017/3/16.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#ifndef enginxServerProcessor_h
#define enginxServerProcessor_h

#include "enginxDefinitions.h"
#include "enginxWorker.h"
#include <map>

ENGINX_NAMESPACE_BEGIN

using namespace std;

class EnginxServerProcessor {
public:
  EnginxServerProcessor(rapidjson::Value& server, EnginxURL const url);
  
  EnginxURL current_url;
  std::string rewrited_url;
  bool can_continue_rewrite;
private:
  std::map<string, string> internal_vars;
  rapidjson::Document current_server;
  bool resolveActions();
  bool resolveInstruction(string instruction);
  bool execInstruction(vector<string>& parts);
  void compileTemplateString(string& template_str);
  void compileInternalVars(vector<string>& parts);
};

ENGINX_NAMESPACE_END

#endif /* enginxServerProcessor_h */
