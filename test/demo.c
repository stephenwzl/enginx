//
//  demo.c
//  xcode
//
//  Created by stephenw on 2017/5/12.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "enginx_dev.h"
#include "Debug.h"
#include "enginx.h"

int main() {
  ENGINX_INTERPRETER* inter =  enginx_create_interpreter();
  enginx_set_current_interpreter(inter);
  FILE *fp = fopen("/Users/stephenw/Desktop/enginx/test/sample.enginx", "r");
  if (fp) {
    if (enginx_compile_file(inter, fp)) {
      enginx_compile_error* error = enginx_get_current_error();
      printf("%s", error->error_msg);
      enginx_compile_error_release(&error);
    }
  }
  debug_print_current_servers();
  enginx_rewrite_url("https://google.com/api/router?hello=world&foo=bar&foo2=bar2");
  char* decode = enginx_url_encode("http://ele.me#百度");
//  enginx_url* url = enginx_url_create("https://google.com");
//  printf("%s\n", enginx_get_request_uri(url));
//  enginx_url_release(&url);
  return 0;
}
