//
//  main.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include <iostream>
#include "time.h"
#include "core/enginx.h"
#include <regex>

using namespace enginx;
using namespace std;

void test_location_equal() {
  string rewrited;
  Enginx::transfer("http://baidu.com/path?url=%e5%93%88%e5%93%88", rewrited);
  cout<<"test location equal:"<<endl;
  cout<< rewrited<<endl;
}

void test_rewrite() {
  string rewrited;
  Enginx::transfer("https://h5.ele.me/shop/#geohash=wtw3dn0w04zkbhsfnqnh3c&id=166657", rewrited);
  cout<<"test rewrite:"<<endl;
  cout<< rewrited<<endl;
}

void test_location_regex() {
  string rewrited;
  Enginx::transfer("http://baidu.com/56789.jpg?mama", rewrited);
  cout<<"test location regex:"<<endl;
  cout<< rewrited<<endl;
}

void test_time() {
  clock_t start, finish;
  start = clock();
  for (int i = 0; i < 100; ++i) {
    string rewrited;
    Enginx::transfer("https://h5.ele.me/shop/#geohash=wtw3dn0w04zkbhsfnqnh3c&id=166657", rewrited);
  }
  finish = clock();
  cout << "time consumed:" << (double)(finish - start)/CLOCKS_PER_SEC <<endl;
}

void test_enginx() {
  string json = "[\
  {\
  \"server_name\":\"h5.ele.me\", \
  \"action\":[\
  ],\
  \"location\":{\
  \"/\":[\
  \"return http://stephenw.cc\"\
  ],\
  \"= /path\":[\
  \"decode $arg_url\",\
  \"return http://google.com?token=$arg_url\"    \
  ],\
  \"^~ /shop\":[\
  \"parse $fragment\",\
  \"var params restaurant_id=$#id{{&target_food_id=$#target_foodID}?}{{&target_skuid=$#target_skuID}?}\",\
  \"encode $var_params\",\
  \"return eleme://restaurant?$var_params\"\
  ],\
  \"~* .*(gif|jpg|jpeg)$\":[\
  \"proxy_pass http://ele.me\"\
  ]\
  }\
  }\
  ]";
  EnginxError error;
  Enginx::load(json.c_str(), error);
  if (error.code) {
    cout << error.message << endl;
  } else {
    test_location_equal();
    test_rewrite();
    test_location_regex();
    test_time();
  }
  std::regex mode;
  bool isvalid = RegexStringValid("*", mode, false);
  cout<<isvalid<<endl;
}

void regex_template() {
  std::string json = "{\"regex\" : \"\\{\\{([^\\s]*?)\\}\\}\"}";
  //  test_enginx();
  std::string s = "\\{\\{([^\\s]*?)\\}\\?\\}";
  std::regex mode = std::regex(s);
  std::string test_string = "http://abc.com/123{{a=$arg_a}?}{{b=$#a}?}";
  std::smatch matches;
  std::regex_search(test_string, matches, mode);
  cout << matches.size() << endl;
  std::string output = "";
  for (auto x : matches) {
    output = x.str();
  }
  cout << output <<endl;
}

int main(int argc, const char * argv[]) {
  test_enginx();
//  regex_template();
}
