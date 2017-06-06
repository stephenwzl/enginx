//
//  ViewController.m
//  iOSDemo
//
//  Created by stephenw on 2017/6/6.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#import "ViewController.h"
#import <enginxiOS/enginxiOS.h>

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  NSString *path = [[NSBundle mainBundle] pathForResource:@"sample" ofType:@"enginx"];
  NSString *config = [[NSString alloc] initWithContentsOfFile:path encoding:NSUTF8StringEncoding error:NULL];
  enginx_load_config_string((char *)config.UTF8String);
  enginx_compile_error *error = enginx_get_current_error();
  if (error != NULL) {
    printf("compile error: %s", error->error_msg);
  }
  enginx_compile_error_release(&error);
}
- (IBAction)doRewrite:(id)sender {
  char* rewrited = enginx_rewrite_url("https://google.com/api/router?hello=world&foo=bar&foo2=bar2");
  printf("%s", rewrited);
  free(rewrited);
}


@end
