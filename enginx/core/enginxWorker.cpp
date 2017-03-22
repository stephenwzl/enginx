//
//  enginxWorker.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginxWorker.h"
#include "enginxServerProcessor.h"

ENGINX_NAMESPACE_BEGIN

bool FieldValid(int offset, int len, size_t url_len) {
  if (offset > url_len) {
    return false;
  }
  if (len == 0) {
    return false;
  }
  return true;
}

string EnginxURL::request_uri() {
  string uri = path;
  if (querystring.length() > 0) {
    uri += "?"+ querystring;
  }
  if (fragment.length() > 0) {
    uri += "#"+ fragment;
  }
  return uri;
}

EnginxURL::EnginxURL(string const absolute_url_string) {
  port = 80;//default port
  enginx_url url;
  http_parser_parse_url(absolute_url_string.c_str(), absolute_url_string.length(), 0, &url);
  absolute_url = absolute_url_string;
  size_t len = absolute_url_string.length();
  for (int i = UF_SCHEMA; i < UF_MAX; ++i) {
    if (!FieldValid(url.field_data[i].off,
                   url.field_data[i].len,len)) {
      continue;
    }
    switch (i) {
      case UF_SCHEMA:
        schema = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_HOST:
        host = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_PATH:
        path = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_PORT:
      {
        string portstr = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        port = atoi(portstr.c_str());
        break;
      }
      case UF_QUERY:
        querystring = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_USERINFO:
        userinfo = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      case UF_FRAGMENT:
        fragment = absolute_url.substr(url.field_data[i].off, url.field_data[i].len);
        break;
      default:
        break;
    }
    
  }
//  if port string is undefined
  if (!FieldValid(url.field_data[UF_PORT].off, url.field_data[UF_PORT].len, len)) {
    if (schema.compare("http") == 0) {
      port = 80;
    } else if (schema.compare("https") == 0) {
      port = 443;
    }
  }
}

bool EnginxURL::isValid() {
  return schema.length() > 0 && host.length() > 0;
}

EnginxURL::~EnginxURL() {}

using namespace rapidjson;
bool is_server_schema_match(Value& server, EnginxURL const url) {
  bool has_schema = server.HasMember(ENGINX_CONFIG_FIELD_SCHEMA);
  if (!has_schema) {
    return true;
  }
  string schema = server[ENGINX_CONFIG_FIELD_SCHEMA].GetString();
  return schema.compare(url.schema) == 0;
}

bool is_server_port_match(Value& server, EnginxURL const url) {
  bool has_port = server.HasMember(ENGINX_CONFIG_FIELD_PORT);
  if (!has_port) {
    return true;
  }
  unsigned int port = server[ENGINX_CONFIG_FIELD_PORT].GetUint();
  return port == url.port;
}

bool is_server_can_response(Value& server, EnginxURL const url) {
  if (!server.HasMember(ENGINX_CONFIG_FIELD_SERVER_NAME)) {
    return false;
  }
  string server_name = server[ENGINX_CONFIG_FIELD_SERVER_NAME].GetString();
  if (server_name.compare(ENGINX_CONFIG_FIELD_DEF_DEFAULT_SERVER) == 0 ||
      server_name.compare(ENGINX_CONFIG_FIELD_DEF_SERVER_WILD) == 0) {
    return is_server_schema_match(server, url) && is_server_port_match(server, url);
  }
  if (server_name.compare(url.host) == 0) {
    return is_server_schema_match(server, url) && is_server_port_match(server, url);
  }
  return false;
}

EnginxWorker::EnginxWorker(string absolute_url_string) {
  rewirted_url = ""; //default url
  current_url = EnginxURL(absolute_url_string);
  if (!current_url.isValid()) {
    //early return if invalid url
    return;
  }
  Document::AllocatorType& allocator = worker_config.GetAllocator();
  worker_config.CopyFrom(EnginxInstance::Instance().current_worker_config, allocator);
  Document::AllocatorType& r_allocator = response_servers.GetAllocator();
  response_servers.SetArray();
  for (Value::ValueIterator itr = worker_config.Begin(); itr < worker_config.End(); ++itr) {
    if (!itr->IsObject()) {
      continue;
    }
    if (is_server_can_response(*itr, current_url)) {
      response_servers.PushBack(*itr, r_allocator);
    }
  }
  if (!response_servers.IsArray() || response_servers.Size() == 0) {
    rewirted_url = current_url.absolute_url;
  } else {
    //TODO:process via multi servers
    EnginxServerProcessor s(response_servers[0], current_url);
    rewirted_url = s.rewrited_url;
  }
}

ENGINX_NAMESPACE_END
