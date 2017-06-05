%{
#include <stdio.h>
#include <string.h>
#include "enginx_dev.h"
#include "enginx_parse.h"

int enginxwrap(void)
{
  return 1;
}

static void increment_line_number(void)
{
  enginx_get_current_interpreter()->current_line_number++;
}
%}
%start COMMENT STRING_LITERAL_STATE
%%
<INITIAL>"server"       return SERVER;
<INITIAL>"domain"       return DOMAIN;
<INITIAL>"scheme"       return SCHEME;
<INITIAL>"port"         return PORT;
<INITIAL>"location"     return LOCATION;
<INITIAL>"("            return LP;
<INITIAL>")"            return RP;
<INITIAL>"{"            return LC;
<INITIAL>"}"            return RC;
<INITIAL>";"            return SEMICOLON;
<INITIAL>":"            return COLON;
<INITIAL>"if"           return IF;
<INITIAL>"encode"       return ENCODE;
<INITIAL>"decode"       return DECODE;
<INITIAL>"return"       return RETURN;
<INITIAL>"match"        return MATCH;
<INITIAL>"parse"        return PARSE;
<INITIAL>"var"          return DEFINE;
<INITIAL>"greater"      return GREATER;
<INITIAL>"equal"        return EQUAL;
<INITIAL>"smaller"      return SMALLER;
<INITIAL>"null" {
  enginxlval.argument = enginx_create_null_value();
  return NULL_VALUE;
}
<INITIAL>$[A-Za-z_][A-Za-z_0-9]* {
  enginxlval.argument = enginx_create_identifier_value(yytext);
  return IDENTIFIER;
}
<INITIAL>#  BEGIN COMMENT;
<INITIAL>[ \t] ;
<INITIAL>\n   { increment_line_number();}
<COMMENT>\n   {
  increment_line_number();
  BEGIN INITIAL;
}
<INITIAL>\" {
  enginx_begin_string_literal();
  BEGIN STRING_LITERAL_STATE;
}
<STRING_LITERAL_STATE>\" {
  char* str = enginx_close_string_literal();
  enginxlval.argument = enginx_create_string_value(str);
  BEGIN INITIAL;
  return STRING_LITERAL;
}
<STRING_LITERAL_STATE>\n        {
    enginx_add_string_literal('\n');
    increment_line_number();
}
<STRING_LITERAL_STATE>\\\"      enginx_add_string_literal('"');
<STRING_LITERAL_STATE>\\n       enginx_add_string_literal('\n');
<STRING_LITERAL_STATE>\\t       enginx_add_string_literal('\t');
<STRING_LITERAL_STATE>\\\\      enginx_add_string_literal('\\');
<STRING_LITERAL_STATE>.         enginx_add_string_literal(enginxtext[0]);
%%