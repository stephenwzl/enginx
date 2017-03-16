//
//  enginxInstance.h
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#ifndef enginxInstance_h
#define enginxInstance_h

#include "enginxDefinitions.h"
#include "vendor/rapidjson/rapidjson.h"
#include "vendor/rapidjson/document.h"
#include "vendor/rapidjson/writer.h"
#include "vendor/rapidjson/stringbuffer.h"
#include <string>

ENGINX_NAMESPACE_BEGIN


class EnginxInstance {
public:
  static EnginxInstance& Instance() {
    static EnginxInstance theSingleton;
    return theSingleton;
  }
  
  /**
   load the config string, in utf-8

   @param config config string
   */
  void load(const char* config);
  
  /**
   test the config after load

   @return if test passed, the config can make sense
   */
  bool test();
  
  /**
   must call reload after test passed
   */
  void reload();
  
  rapidjson::Document current_worker_config;
private:
  EnginxInstance();                            // ctor hidden
  ~EnginxInstance();                           // dtor hidden
  std::string current_config;
  std::string testing_config;
  rapidjson::Document testing_worker_config;
};

ENGINX_NAMESPACE_END

#endif /* enginxInstance_h */
