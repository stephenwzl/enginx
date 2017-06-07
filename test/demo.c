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
#include "time.h"

int main() {
  
  ENGINX_INTERPRETER* inter =  enginx_create_interpreter();
  enginx_set_current_interpreter(inter);
  //change your sample file path here
  char* sampleFilePath = "/Users/stephenw/Desktop/enginx/test/sample.enginx";
  FILE *fp = fopen(sampleFilePath, "r");
  if (fp) {
    if (enginx_compile_file(inter, fp)) {
      enginx_compile_error* error = enginx_get_current_error();
      printf("%s", error->error_msg);
      enginx_compile_error_release(&error);
    }
  }
  debug_print_current_servers();
  char* rewrited = enginx_rewrite_url("https://google.com/api/router?hello=world&foo=bar&foo2=bar2");
  printf("%s\n", rewrited);
  rewrited = enginx_rewrite_url("https://ele.me");
  printf("%s\n", rewrited);
  clock_t start, finish;
  start = clock();
  for (int i = 0; i < 10000; ++i) {
    char* rewrited = enginx_rewrite_url("https://google.com/api/router?hello=world&foo=bar&foo2=bar2");
    free(rewrited);
  }
  finish = clock();
  printf("time consumed: %lf\n", (double)(finish - start)/CLOCKS_PER_SEC);
  return 0;
}
