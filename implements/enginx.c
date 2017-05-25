//
//  enginx.c
//  xcode
//
//  Created by stephenw on 2017/5/16.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginx.h"
#include "enginx_dev.h"
#include "http_parser.h"
#include "Mem.h"
#include <string.h>
#include <regex.h>

const char* ENGINX_CONFIG_VAR_DEF_REQUEST_URI = "$request_uri";
const char* ENGINX_CONFIG_VAR_DEF_HOST = "$host";
const char* ENGINX_CONFIG_VAR_DEF_SCHEME = "$scheme";
const char* ENGINX_CONFIG_VAR_DEF_QUERY_STRING = "$query_string";
const char* ENGINX_CONFIG_VAR_DEF_FRAGMENT = "$fragment";
const char* ENGINX_CONFIG_VAR_DEF_PATH = "$path";

static enginx_compile_error* current_compile_error = NULL;

void enginx_load_config_string(char* config_str)
{
  ENGINX_INTERPRETER* current_inter = enginx_get_current_interpreter();
  if (current_inter) {
    enginx_reset_interpreter(current_inter);
  }
  current_inter = enginx_create_interpreter();
  enginx_set_current_interpreter(current_inter);
  enginx_compile_string(current_inter, config_str);
}

void enginx_load_config_file(FILE* fp)
{
  ENGINX_INTERPRETER* current_inter = enginx_get_current_interpreter();
  if (current_inter) {
    enginx_reset_interpreter(current_inter);
  }
  current_inter = enginx_create_interpreter();
  enginx_set_current_interpreter(current_inter);
  enginx_compile_file(current_inter, fp);
}

enginx_compile_error* enginx_compile_error_create(int code, char* message)
{
  enginx_compile_error* error;
  error = (enginx_compile_error*)malloc(sizeof(enginx_compile_error));
  error->code = code;
  error->error_msg = (char*)malloc(strlen(message)*sizeof(char)+1);
  strcpy(error->error_msg, message);
  current_compile_error = error;
  return error;
}

void enginx_compile_error_release(enginx_compile_error** error)
{
  if (error == NULL || *error == NULL) {
    return;
  }
  free((*error)->error_msg);
  (*error)->error_msg = NULL;
  free(*error);
  *error = NULL;
  current_compile_error = NULL;
}

enginx_compile_error* enginx_get_current_error()
{
  return current_compile_error;
}

//helper method
//create string from exist index and length
char* create_url_part(char* absolute_url, uint16_t offset, uint16_t length)
{
  char* return_str = (char*)malloc((length + 1) * sizeof(char));
  char* start = &absolute_url[offset];
  memcpy(return_str, start, length);
  return_str[length] = '\0';
  return return_str;
}

int url_field_valid(uint16_t offset, uint16_t len, size_t url_len)
{
  if (offset > url_len) {
    return 0;
  }
  if (len == 0) {
    return 0;
  }
  return 1;
}

enginx_url* enginx_url_create(char* absolute_url_string)
{
  struct http_parser_url parser_url;
  http_parser_parse_url(absolute_url_string, strlen(absolute_url_string), 0, &parser_url);
  size_t url_len = strlen(absolute_url_string);
  enginx_url* url;
  url = (enginx_url*)malloc(sizeof(enginx_url));
  url->scheme = NULL;
  url->host = NULL;
  url->path = NULL;
  url->fragment = NULL;
  url->querystring = NULL;
  url->userinfo = NULL;
  url->absolute_url = NULL;
  url->port = 80;//default port
  for (int i = UF_SCHEMA; i < UF_MAX; ++i) {
    if (!url_field_valid(parser_url.field_data[i].off, parser_url.field_data[i].len, url_len)) {
      continue;
    }
    char* url_part = create_url_part(absolute_url_string, parser_url.field_data[i].off, parser_url.field_data[i].len);
    switch (i) {
      case UF_SCHEMA:
        url->scheme = url_part;
        break;
      case UF_HOST:
        url->host = url_part;
        break;
      case UF_PATH:
        url->path = url_part;
        break;
      case UF_PORT:
      {
        char* port_str = url_part;
        url->port = atoi(port_str);
        free(port_str);
        port_str = NULL;
        break;
      }
      case UF_QUERY:
        url->querystring = url_part;
        break;
      case UF_USERINFO:
        url->userinfo = url_part;
        break;
      case UF_FRAGMENT:
        url->fragment = url_part;
        break;
      default:
        break;
    }
  }
  if (!url_field_valid(parser_url.field_data[UF_PORT].off, parser_url.field_data[UF_PORT].len, url_len)) {
    if (strcmp(url->scheme, "http") == 0) {
      url->port = 80;
    } else if (strcmp(url->scheme, "https") == 0) {
      url->port = 443;
    }
  }
  if (url->path == NULL) {
    url->path = (char*)malloc(2);
    strcpy(url->path, "/");
  }
  url->absolute_url = (char*)malloc(strlen(absolute_url_string) + 1);
  strcpy(url->absolute_url, absolute_url_string);
  return url;
}

char* enginx_get_request_uri(enginx_url* url)
{
  char* return_str= NULL;
  size_t uri_len = strlen(url->path) + 1;
  return_str = (char*)malloc(uri_len);
  memcpy(return_str, url->path, uri_len);
  
  if (url->querystring != NULL) {
    size_t origin_len = uri_len;
    uri_len += (strlen("?") + strlen(url->querystring));
    return_str = realloc(return_str, uri_len);
    return_str[origin_len - 1] = '?';
    memcpy(&return_str[origin_len], url->querystring, strlen(url->querystring));
  }
  if (url->fragment != NULL) {
    size_t origin_len = uri_len;
    uri_len += (strlen("#") + strlen(url->fragment)) * sizeof(char);
    return_str = realloc(return_str, uri_len);
    return_str[origin_len - 1] = '#';
    memcpy(&return_str[origin_len], url->fragment, strlen(url->fragment));
  }
  return_str[uri_len - 1] = '\0';
  return return_str;
}

int enginx_url_is_valid(enginx_url* url)
{
  return url->scheme != NULL && url->host != NULL;
}

void enginx_url_release(enginx_url** url)
{
  if (url == NULL || *url == NULL) {
    return;
  }
  if ((*url)->host != NULL) {
    free((*url)->host);
    (*url)->host = NULL;
  }
  if ((*url)->scheme != NULL) {
    free((*url)->scheme);
    (*url)->scheme = NULL;
  }
  if ((*url)->path != NULL) {
    free((*url)->path);
    (*url)->path = NULL;
  }
  if ((*url)->fragment != NULL) {
    free((*url)->fragment);
    (*url)->fragment = NULL;
  }
  if ((*url)->querystring != NULL) {
    free((*url)->querystring);
    (*url)->querystring = NULL;
  }
  if ((*url)->userinfo != NULL) {
    free((*url)->userinfo);
    (*url)->userinfo = NULL;
  }
  if ((*url)->absolute_url != NULL) {
    free((*url)->absolute_url);
    (*url)->absolute_url = NULL;
  }
  free(*url);
  *url = NULL;
}

#pragma mark - pre definitions of functions
char* enginx_server_process(char* absolute_url);
char* enginx_copy_string(char* str)
{
  char* return_str = (char*)malloc(strlen(str)+1);
  strcpy(return_str, str);
  return return_str;
}

char* enginx_rewrite_url(char* absolute_url)
{
  if (enginx_get_current_interpreter() == NULL ||
      enginx_get_current_interpreter()->server_list == NULL) {
    return enginx_copy_string(absolute_url);
  }
  return enginx_server_process(absolute_url);
}

#pragma mark - server process
int enginx_is_server_response(ENGINX_SERVER* server, enginx_url* url)
{
  return strcmp(server->domain->u.string_value, url->host) == 0 &&
  strcmp(server->scheme->u.string_value, url->scheme) == 0 &&
  server->port == url->port;
}

char* enginx_location_process(ENGINX_SERVER* server, enginx_url* url);

char* enginx_server_process(char* absolute_url)
{
  enginx_url* url = enginx_url_create(absolute_url);
  if (!enginx_url_is_valid(url)) {
    return enginx_copy_string(absolute_url);
  }
  ENGINX_INTERPRETER* interpreter = enginx_get_current_interpreter();
  ENGINX_SERVER* server_pos;
  for (server_pos = interpreter->server_list; server_pos; server_pos = server_pos->next) {
    if (enginx_is_server_response(server_pos, url)) {
      char* return_str = enginx_location_process(server_pos, url);
      enginx_url_release(&url);
      return return_str;
    }
  }
  enginx_url_release(&url);
  return enginx_copy_string(absolute_url);
}

#pragma mark - location process
char* enginx_exec_location_statements(ENGINX_LOCATION* location, enginx_url* url);

char* enginx_location_process(ENGINX_SERVER* server, enginx_url* url)
{
  ENGINX_LOCATION* location;
  for (location = server->locations; location; location = location->next) {
    regex_t regex;
    int compile_failed = regcomp(&regex, location->matcher->u.string_value, REG_EXTENDED);
    if (compile_failed) {
      continue;
    }
    regmatch_t matches[1];
    int status = regexec(&regex, url->path, 1, matches, 0);
    if (status == 0) {
      regfree(&regex);
      return enginx_exec_location_statements(location, url);//return location statements
    } else {
      regfree(&regex);
      continue;
    }
  }
  return NULL;
}

enginx_dictionary* enginx_generate_global_variables(enginx_url* url);
enginx_dictionary* enginx_generate_query_variables(enginx_url* url);
enginx_dictionary* enginx_dictionary_cat(enginx_dictionary* head, enginx_dictionary* tail);
//if not null, means return
char* enginx_exec_statements(ENGINX_STATEMENT* statement, enginx_dictionary* global_values, enginx_dictionary* internal_variables);

char* enginx_exec_location_statements(ENGINX_LOCATION* location, enginx_url* url)
{
  enginx_dictionary* global_variables = enginx_generate_global_variables(url);
  enginx_dictionary* query_variables = enginx_generate_query_variables(url);
  global_variables = enginx_dictionary_cat(global_variables, query_variables);
  //placeholder key&value
  enginx_dictionary* internal_variables = NULL;
  ENGINX_STATEMENT_LIST* state_list;
  for (state_list = location->location_statements; state_list; state_list = state_list->next) {
    char* return_val = enginx_exec_statements(state_list->statement, global_variables, internal_variables);
    if (return_val != NULL) {
      return return_val;
    }
  }
  enginx_release_dictionary(global_variables);
  return NULL;
}

enginx_dictionary* enginx_dictionary_cat(enginx_dictionary* head, enginx_dictionary* tail)
{
  enginx_dictionary* pos;
  for (pos = head; pos->next; pos = pos->next);
  pos->next = tail;
  return head;
}

char* enginx_value_for_key(enginx_dictionary* dict, char* key)
{
  enginx_dictionary* pos;
  for (pos = dict; pos; pos = pos->next) {
    if (strcmp(key, pos->key) == 0) {
      return pos->value;
    }
  }
  return NULL;
}

enginx_dictionary* enginx_generate_global_variables(enginx_url* url)
{
  enginx_dictionary* global_variables = enginx_dictionary_create(ENGINX_CONFIG_VAR_DEF_HOST, url->host);
  enginx_set_value_for_key(global_variables, ENGINX_CONFIG_VAR_DEF_SCHEME, url->scheme);
  enginx_set_value_for_key(global_variables, ENGINX_CONFIG_VAR_DEF_QUERY_STRING, url->querystring);
  enginx_set_value_for_key(global_variables, ENGINX_CONFIG_VAR_DEF_FRAGMENT, url->fragment);
  enginx_set_value_for_key(global_variables, ENGINX_CONFIG_VAR_DEF_REQUEST_URI, enginx_get_request_uri(url));
  enginx_set_value_for_key(global_variables, ENGINX_CONFIG_VAR_DEF_PATH, url->path);
  return global_variables;
}

char** enginx_generate_key_value_pairs(char* str)
{
  char **pairs = (char**)malloc(sizeof(char*) * 2);
  char spliter = '=';
  pairs[0] = NULL;
  pairs[1] = NULL;
  int index = -1;
  size_t str_len = strlen(str);
  for (int i = 0; i < str_len; ++i) {
    if (str[i] == spliter) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    pairs[0] = (char*)malloc(str_len + 1);
    strcpy(pairs[0], str);
    return pairs;
  }
  pairs[0] = (char*)malloc(index + 1);
  strncpy(pairs[0], str, index + 1);
  pairs[0][index] = '\0';
  if (str_len - 1 - index > 0) {
    pairs[1] = (char*)malloc(str_len-index);
    strncpy(pairs[1], &str[index + 1], str_len - 1 - index);
  }
  return pairs;
}

enginx_dictionary* enginx_generate_query_variables(enginx_url* url)
{
  if (url->querystring == NULL) {
    return NULL;
  }
  enginx_dictionary* dict = NULL;
  char* spliter = "&";
  char buffer[256];
  char* first = strtok(url->querystring, spliter);
  while (first != NULL) {
    char** key_value = enginx_generate_key_value_pairs(first);
    if (key_value[0] != NULL) {
      sprintf(buffer, "$arg_%s", key_value[0]);
      if (dict == NULL) {
        dict = enginx_dictionary_create(buffer, key_value[1]);
      } else {
        enginx_set_value_for_key(dict, buffer, key_value[1]);
      }
    }
    if (key_value) {
      if (key_value[0]) {
        free(key_value[0]);
      }
      if (key_value[1]) {
        free(key_value[1]);
      }
      free(key_value);
      key_value = NULL;
    }
    first = strtok(NULL, spliter);
  }
  
  return dict;
}

#pragma mark - enginx dictionary methods
enginx_dictionary* enginx_dictionary_create(const char* key, const char* value)
{
  enginx_dictionary* dict;
  dict = (enginx_dictionary*)malloc(sizeof(enginx_dictionary));
  dict->key = (char*)malloc((strlen(key) + 1) * sizeof(char));
  strcpy(dict->key, key);
  if (value == NULL) {
    dict->value = (char*)malloc(1);
    dict->value[0] = '\0';
    dict->next = NULL;
    return dict;
  }
  dict->value = (char*)malloc((strlen(value) + 1) * sizeof(char));
  strcpy(dict->value, value);
  dict->next = NULL;
  return dict;
}

enginx_dictionary* enginx_set_value_for_key(enginx_dictionary* dict,const char* key,const char* value)
{
  enginx_dictionary* pos;
  enginx_dictionary* tail = NULL;
  for (pos = dict; pos; pos = pos->next) {
    if (strcmp(pos->key, key) == 0) {
      pos->value = (char*)realloc(pos->value, (strlen(value) + 1) * sizeof(char));
      strcpy(pos->value, value);
      return dict;
    }
    tail = pos;
  }
  pos = enginx_dictionary_create(key, value);
  tail->next = pos;
  return dict;
}

void enginx_release_dictionary(enginx_dictionary* dict)
{
  enginx_dictionary* pos = dict;
  while (pos) {
    free(pos->key);
    free(pos->value);
    pos->key = NULL;
    pos->value = NULL;
    pos = pos->next;
  }
}

#pragma mark - exec statements
char* enginx_exec_expression(ENGINX_EXPRESSION* expression,
                             enginx_dictionary* global_values,
                             enginx_dictionary* internal_variables);

char* enginx_exec_if_statement(ENGINX_IF_STATMENT* if_statement,
                               enginx_dictionary* global_values,
                               enginx_dictionary* internal_values);

char* enginx_exec_statements(ENGINX_STATEMENT* statement,
                             enginx_dictionary* global_values,
                             enginx_dictionary* internal_variables)
{
  switch (statement->type) {
    case EXPRESSION_STATEMENT:
    return enginx_exec_expression(statement->u.expression, global_values, internal_variables);
    case IF_STATEMENT:
    return enginx_exec_if_statement(statement->u.if_statement, global_values, internal_variables);
    break;
    case UNKNOWN_STATEMENT:
    default:
    break;
  }
  return NULL;
}

char* enginx_exec_expression(ENGINX_EXPRESSION* expression,
                             enginx_dictionary* global_values,
                             enginx_dictionary* internal_variables)
{
  char buffer[128];
  switch (expression->type) {
    case ENCODE_EXPRESSION:
    {
      if (expression->list->value &&
          expression->list->value->type == ENGINX_IDENTIFIER_VALUE) {
        char* val = enginx_value_for_key(global_values, expression->list->value->u.string_value);
        if (val != NULL) {
          char* res = enginx_url_encode(val);
          enginx_set_value_for_key(global_values, expression->list->value->u.string_value, res);
          free(res);
        }
      }
      break;
    }
    case DECODE_EXPRESSION:
    {
      char* val = enginx_value_for_key(global_values, expression->list->value->u.string_value);
      if (val == NULL) {
        enginx_value_for_key(internal_variables, expression->list->value->u.string_value);
      }
      if (val != NULL) {
        char* res = enginx_url_decode(val);
        enginx_set_value_for_key(global_values, expression->list->value->u.string_value, res);
        free(res);
      }
      break;
    }
    case MATCH_EXPRESSION:
    {
      ENGINX_VALUE* first_arg = expression->list->value;
      ENGINX_VALUE* second_arg = expression->list->next->value;
      char* val = enginx_value_for_key(global_values, first_arg->u.identifier_value);
      if (val == NULL) {
        enginx_value_for_key(internal_variables, first_arg->u.identifier_value);
      }
      if (val != NULL && second_arg->u.string_value != NULL) {
        regex_t regexCompiled;
        regmatch_t matchGroups[10];
        if (regcomp(&regexCompiled, second_arg->u.string_value, REG_EXTENDED)) {
          //invalid regex string
          break;
        }
        if (regexec(&regexCompiled, val, 10, matchGroups, 0) == 0) {
          unsigned int i = 0;
          for (i = 0; i < 10; i++) {
            regmatch_t group = matchGroups[i];
            if (group.rm_so == (size_t) -1) {
              break;
            }
            char* source_cp = (char*)malloc(strlen(val) + 1);
            strcpy(source_cp, val);
            source_cp[group.rm_eo] = '\0';
            sprintf(buffer, "$%d", i);
            char* key = malloc(strlen(buffer) + 1);
            strcpy(key, buffer);
            if (internal_variables != NULL) {
              enginx_set_value_for_key(internal_variables, key, &source_cp[group.rm_so]);
            } else {
              internal_variables = enginx_dictionary_create(key, &source_cp[group.rm_so]);
            }
            free(source_cp);
            free(key);
          }
        }
      }
      break;
    }
    case RETURN_EXPRESSION:
    {
      
    }
    case DEFINE_EXPRESSION:
    case PARSE_EXPRESSION:
    default:
    break;
  }
  return NULL;
}

char* enginx_exec_if_statement(ENGINX_IF_STATMENT* if_statement,
                               enginx_dictionary* global_values,
                               enginx_dictionary* internal_values)
{
  return NULL;
}
