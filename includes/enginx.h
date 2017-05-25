//
//  enginx.h
//  xcode
//
//  Created by stephenw on 2017/5/16.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#ifndef enginx_h
#define enginx_h

#include <stdio.h>

extern const char* ENGINX_CONFIG_VAR_DEF_REQUEST_URI;
extern const char* ENGINX_CONFIG_VAR_DEF_HOST;
extern const char* ENGINX_CONFIG_VAR_DEF_SCHEME;
extern const char* ENGINX_CONFIG_VAR_DEF_QUERY_STRING;
extern const char* ENGINX_CONFIG_VAR_DEF_FRAGMENT;
extern const char* ENGINX_CONFIG_VAR_DEF_PATH;

typedef struct enginx_compile_error_node enginx_compile_error;
typedef struct enginx_url_node  enginx_url;
typedef struct enginx_dictionary_node enginx_dictionary;
/**
 load config from str

 @param config_str manage str memory yourself
 */
void enginx_load_config_string(char* config_str);


/**
 load config from file

 @param fp manage file pointer yourself
 */
void enginx_load_config_file(FILE* fp);


/**
 enginx error 
 */
struct enginx_compile_error_node {
  int       code;
  char*     error_msg;
};

struct enginx_url_node {
  char* scheme;
  unsigned int port;
  char* host;
  char* path;
  char* querystring;
  char* fragment;
  char* userinfo;
  char* absolute_url;
};

// just str <=> str dict
struct enginx_dictionary_node {
  char* key;
  char* value;
  struct enginx_dictionary_node* next;
};

#pragma mark - enginx_error methods
/**
 creat a compile error

 @param code error code
 @param message error message
 @return returns a compile error
 */
enginx_compile_error* enginx_compile_error_create(int code, char* message);

/**
 release a compile error's memory

 @param error pointer that points to the error
 */
void enginx_compile_error_release(enginx_compile_error** error);

/**
 get current compiled error, may be NULL

 @return error
 */
enginx_compile_error* enginx_get_current_error();

#pragma mark - enginx_url methods
enginx_url* enginx_url_create(char* absolute_url_string);
char* enginx_get_request_uri(enginx_url* url);
int enginx_url_is_valid(enginx_url* url);
void enginx_url_release(enginx_url** url);

#pragma mark - enginx dictionary methods
enginx_dictionary* enginx_dictionary_create(const char* key, const char* value);
enginx_dictionary* enginx_set_value_for_key(enginx_dictionary* dict, const char* key,const char* value);
void enginx_release_dictionary(enginx_dictionary* dict);

#pragma mark - enginx entries

/**
 rewrite a url

 @param absolute_url absolute url string
 @return manage the return string memory yourself
 */
char* enginx_rewrite_url(char* absolute_url);

#endif /* enginx_h */
