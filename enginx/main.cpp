//
//  main.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include <iostream>
#include "core/enginxInstance.h"
#include "core/enginx.h"

using namespace enginx;
using namespace std;

int main(int argc, const char * argv[]) {
  const char *json = "[{\"server_name\":\"enginx\"}]";
  EnginxInstance::Instance().load(json);
  bool tested = EnginxInstance::Instance().test();
  cout<<tested<<endl;
}
