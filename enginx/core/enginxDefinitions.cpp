//
//  enginxDefinitions.cpp
//  enginx
//
//  Created by stephenw on 2017/3/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "enginxDefinitions.h"

int const ENGINX_ERROR_BAD_PARAMETER = 10400;
const char* ENGINX_CONFIG_FIELD_SERVER_NAME = "server_name";
const char* ENGINX_CONFIG_FIELD_SCHEMA = "scheme";
const char* ENGINX_CONFIG_FIELD_PORT = "listen";
const char* ENGINX_CONFIG_FIELD_ACTION = "action";
const char* ENGINX_CONFIG_FIELD_LOCATION = "location";

const char* ENGINX_CONFIG_FIELD_DEF_DEFAULT_SERVER = "default_server";
const char* ENGINX_CONFIG_FIELD_DEF_SERVER_WILD = "*";

const char* ENGINX_CONFIG_VAR_DEF_REQUEST_URI = "$request_uri";
const char* ENGINX_CONFIG_VAR_DEF_HOST = "$host";
const char* ENGINX_CONFIG_VAR_DEF_SCHEME = "$scheme";
const char* ENGINX_CONFIG_VAR_DEF_QUERY_STRING = "$query_string";
const char* ENGINX_CONFIG_VAR_DEF_FRAGMENT = "$fragment";
const char* ENGINX_CONFIG_VAR_DEF_PATH = "$path";

const char* ENGINX_CONFIG_INSTRUCTION_DECODE = "decode";
const char* ENGINX_CONFIG_INSTRUCTION_ENCODE = "encode";
const char* ENGINX_CONFIG_INSTRUCTION_REWRITE = "rewrite";
const char* ENGINX_CONFIG_INSTRUCTION_PROXY_PASS = "proxy_pass";
const char* ENGINX_CONFIG_INSTRUCTION_RETURN = "return";
const char* ENGINX_CONFIG_INSTRUCTION_TEMPORARILY = "temporarily";

const char* ENGINX_CONFIG_OPERATOR_EQUAL = "=";
//const char* ENGINX_CONFIG_OPERATIR_STRING = "/xx/xx";
const char* ENGINX_CONFIG_OPERATOR_NORMAL_HEAD = "^~";
const char* ENGINX_CONFIG_OPERATOR_REG_NO_CASE_SENSITIVE = "~";
const char* ENGINX_CONFIG_OPERATOR_REG_CASE_SENSITIVE = "~*";
const char* ENGINX_CONFIG_OPERATOR_WILD_COMPARE = "/";

ENGINX_NAMESPACE_BEGIN

EnginxError::EnginxError(const char*msg, int code) {
  message = msg;
  this->code = code;
}

bool RegexStringValid(const std::string& s, std::regex& mode, bool ignoreCase) {
  try {
    if (ignoreCase) {
      mode = std::regex(s, std::regex::icase);
    } else {
      mode = std::regex(s);
    }
  } catch (const std::regex_error& error) {
    return false;
  }
  return true;
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c) {
  std::string::size_type pos1, pos2;
  pos2 = s.find(c);
  pos1 = 0;
  while(std::string::npos != pos2)
  {
    v.push_back(s.substr(pos1, pos2-pos1));
    
    pos1 = pos2 + c.size();
    pos2 = s.find(c, pos1);
  }
  if(pos1 != s.length())
    v.push_back(s.substr(pos1));
}

unsigned char ToHex(unsigned char x)
{
  return  x > 9 ? x + 55 : x + 48;
}

unsigned char FromHex(unsigned char x)
{
  unsigned char y;
  if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
  else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
  else if (x >= '0' && x <= '9') y = x - '0';
//  else assert(0);
//  asserts will call crash in Android
  else y = '0';
  return y;
}

std::string UrlEncode(const std::string& str)
{
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++)
  {
    if (isalnum((unsigned char)str[i]) ||
        (str[i] == '-') ||
        (str[i] == '_') ||
        (str[i] == '.') ||
        (str[i] == '~'))
      strTemp += str[i];
    else if (str[i] == ' ')
      strTemp += "+";
    else
    {
      strTemp += '%';
      strTemp += ToHex((unsigned char)str[i] >> 4);
      strTemp += ToHex((unsigned char)str[i] % 16);
    }
  }
  return strTemp;
}

std::string UrlDecode(const std::string& str)
{
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++)
  {
    if (str[i] == '+') strTemp += ' ';
    else if (str[i] == '%')
    {
//      assert(i + 2 < length);
//  asserts will call crash in Android
      if (i + 2 >= length) {
        break;
      }
      unsigned char high = FromHex((unsigned char)str[++i]);
      unsigned char low = FromHex((unsigned char)str[++i]);
      strTemp += high*16 + low;
    }
    else strTemp += str[i];
  }
  return strTemp;
}
ENGINX_NAMESPACE_END
