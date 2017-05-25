%{
#include <stdio.h>
#include <string.h>
#include "enginx.h"
#include "y.tab.h"

int yywrap(void)
{
  return 1;
}

static void increment_line_number(void)
{
  enginx_get_current_interpreter()->current_line_number++;
}
%}
%start COMMENT
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

<INITIAL>#  BEGIN COMMENT;
<INITIAL>[ \t] ;
<INITIAL>\n   { increment_line_number();}
<COMMENT>\n   {
  increment_line_number();
  BEGIN INITIAL;
}

<INITIAL>[^\s]*     {
  yylval.argument = enginx_create_string_value(yytext);
  return STRING_LITERAL;
}


%%