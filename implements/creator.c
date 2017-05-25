//
//  creator.c
//  xcode
//
//  Created by stephenw on 2017/5/15.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#include "Mem.h"
#include "enginx_dev.h"
#include "enginx.h"
#include "Debug.h"
#include <string.h>
#include <ctype.h>

static ENGINX_INTERPRETER *enginx_current_interpreter;

void enginx_set_current_interpreter(ENGINX_INTERPRETER* inter)
{
  enginx_current_interpreter = inter;
}

ENGINX_INTERPRETER* enginx_get_current_interpreter()
{
  return enginx_current_interpreter;
}

ENGINX_INTERPRETER* enginx_create_interpreter()
{
  MEM_Storage storage;
  ENGINX_INTERPRETER *interpreter;
  storage = MEM_open_storage(0);
  interpreter = MEM_storage_malloc(storage, sizeof(struct enginx_interpreter_node));
  interpreter->interpreter_storage = storage;
  interpreter->execute_storage = NULL;
  interpreter->current_line_number = 1;
  interpreter->server_list = NULL;
  
  enginx_set_current_interpreter(interpreter);
  return interpreter;
}

int enginx_compile_file(ENGINX_INTERPRETER* interpreter, FILE *fp)
{
  extern int yyparse(void);
  extern FILE *yyin;
  enginx_set_current_interpreter(interpreter);
  yyin = fp;
  if (yyparse()) {
#ifdef DEBUG
    fprintf(stderr, "yyparse error occured\n");
#endif
    return 1;
  }
  return 0;
}

int enginx_compile_string(ENGINX_INTERPRETER* interpreter, char* str)
{
  typedef struct yy_buffer_state *YY_BUFFER_STATE;
  extern int yyparse();
  extern YY_BUFFER_STATE yy_scan_string(char * str);
  extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
  YY_BUFFER_STATE buffer = yy_scan_string(str);
  if (yyparse()) {
#ifdef DEBUG
    fprintf(stderr, "yyparse error occured\n");
#endif
    yy_delete_buffer(buffer);
    return 1;
  }
  yy_delete_buffer(buffer);
  return 0;
}

void enginx_reset_interpreter(ENGINX_INTERPRETER *interpreter)
{
  if (interpreter->execute_storage) {
    MEM_dispose_storage(interpreter->execute_storage);
  }
  MEM_dispose_storage(interpreter->interpreter_storage);
}


void* enginx_malloc(size_t size)
{
  void* p;
  ENGINX_INTERPRETER* inter;
  inter = enginx_get_current_interpreter();
  p = MEM_storage_malloc(inter->interpreter_storage, size);
  return p;
}

ENGINX_EXPRESSION* enginx_create_expression(ENGINX_EXPRESSION_TYPE type, ENGINX_ARGUMENT_LIST* argument_list)
{
  ENGINX_EXPRESSION *exp;
  exp = enginx_malloc(sizeof(ENGINX_EXPRESSION));
  exp->type = type;
  exp->line_number = enginx_get_current_interpreter()->current_line_number;
  exp->list = argument_list;
  return exp;
}


ENGINX_ARGUMENT_LIST* enginx_create_argument_list(ENGINX_VALUE* value)
{
  ENGINX_ARGUMENT_LIST* alist;
  alist = enginx_malloc(sizeof(ENGINX_ARGUMENT_LIST));
  alist->value = value;
  alist->next = NULL;
  return alist;
}

ENGINX_ARGUMENT_LIST* enginx_chain_argument_list(ENGINX_ARGUMENT_LIST* alist, ENGINX_VALUE* value)
{
  ENGINX_ARGUMENT_LIST *pos;
  for (pos = alist; pos->next; pos = pos->next);
  pos->next = enginx_create_argument_list(value);
  return pos;
}

ENGINX_STATEMENT* enginx_create_normal_statement(ENGINX_EXPRESSION* expression)
{
  ENGINX_STATEMENT* st;
  st = enginx_malloc(sizeof(ENGINX_STATEMENT));
  st->type = EXPRESSION_STATEMENT;
  st->line_number = enginx_get_current_interpreter()->current_line_number;
  st->u.expression = expression;
  return st;
}

ENGINX_STATEMENT* enginx_create_if_statement(ENGINX_EXPRESSION* condition, ENGINX_BLOCK* block)
{
  ENGINX_IF_STATMENT* if_st;
  if_st = enginx_malloc(sizeof(ENGINX_IF_STATMENT));
  if_st->condition = condition;
  if_st->then_block = block;
  ENGINX_STATEMENT* st;
  st = enginx_malloc(sizeof(ENGINX_STATEMENT));
  st->type = IF_STATEMENT;
  st->line_number = enginx_get_current_interpreter()->current_line_number;
  st->u.if_statement = if_st;
  return st;
}

ENGINX_STATEMENT_LIST* enginx_create_statement_list(ENGINX_STATEMENT* statement)
{
  ENGINX_STATEMENT_LIST* st_list;
  st_list = enginx_malloc(sizeof(ENGINX_STATEMENT_LIST));
  st_list->statement = statement;
  st_list->next = NULL;
  return st_list;
}

ENGINX_STATEMENT_LIST* enginx_chain_statement_list(ENGINX_STATEMENT_LIST* list, ENGINX_STATEMENT* statement)
{
  ENGINX_STATEMENT_LIST* pos;
  for (pos = list; pos->next; pos = pos->next);
  pos->next = enginx_create_statement_list(statement);
  return pos;
}

ENGINX_BLOCK* enginx_create_block(ENGINX_STATEMENT_LIST* list)
{
  ENGINX_BLOCK* block;
  block = enginx_malloc(sizeof(ENGINX_BLOCK));
  block->statement_list = list;
  return block;
}

//TODO: 检查所有的 char* 是否从默认的 memory controller中分配内存

ENGINX_LOCATION* enginx_create_location(ENGINX_VALUE* matcher, ENGINX_STATEMENT_LIST* statement_list)
{
  ENGINX_LOCATION* location;
  location = enginx_malloc(sizeof(ENGINX_LOCATION));
  location->matcher = matcher;
  location->location_statements = statement_list;
  location->next = NULL;
  return location;
}

ENGINX_LOCATION* enginx_chain_location(ENGINX_LOCATION* list, ENGINX_LOCATION* location)
{
  ENGINX_LOCATION* pos;
  for (pos = list; pos->next; pos = pos->next);
  pos->next = location;
  return pos;
}

ENGINX_SERVER* enginx_create_server(ENGINX_VALUE* domain, ENGINX_VALUE* scheme, unsigned int port, ENGINX_LOCATION* locations)
{
  ENGINX_SERVER* server;
  server = enginx_malloc(sizeof(ENGINX_SERVER));
  server->domain = domain;
  server->scheme = scheme;
  server->port = port;
  server->locations = locations;
  server->next = NULL;
  return server;
}

ENGINX_SERVER* enginx_chain_server(ENGINX_SERVER* list, ENGINX_SERVER* server)
{
  ENGINX_SERVER* pos;
  for (pos = list; pos->next; pos = pos->next);
  pos->next = server;
  return pos;
}

char* enginx_create_string(char* str)
{
  char* new_str;
  new_str = enginx_malloc(strlen(str)+1);
  strcpy(new_str, str);
  return new_str;
}

ENGINX_VALUE* enginx_create_string_value(char* str)
{
  ENGINX_VALUE* val;
  val = enginx_malloc(sizeof(ENGINX_VALUE));
  val->type = ENGINX_STRING_VALUE;
  val->u.string_value = enginx_create_string(str);
  return val;
}

ENGINX_VALUE* enginx_create_int_value(int value)
{
  ENGINX_VALUE* val;
  val = enginx_malloc(sizeof(ENGINX_VALUE));
  val->type = ENGINX_INT_VALUE;
  val->u.int_value = value;
  return val;
}

ENGINX_VALUE* enginx_create_identifier_value(char* str)
{
  ENGINX_VALUE* val;
  val = enginx_malloc(sizeof(ENGINX_VALUE));
  val->type = ENGINX_IDENTIFIER_VALUE;
  val->u.identifier_value = enginx_create_string(str);
  return val;
}

int enginx_value_to_integer(ENGINX_VALUE* value)
{
  if (value->type == ENGINX_BOOLEAN_VALUE) {
    return value->u.boolean_value;
  } else if (value->type == ENGINX_INT_VALUE) {
    return value->u.int_value;
  }
  return atoi(value->u.string_value);
}

int yyerror(char const *str)
{
  extern char *yytext;
#ifdef DEBUG
  printf("line: %d\n",  enginx_get_current_interpreter()->current_line_number);
  fprintf(stderr, "parser error near %s\n", yytext);
#endif
  char msg_buffer[256];
  snprintf(msg_buffer, 256, "%s at line %d: \"%s\"", str, enginx_get_current_interpreter()->current_line_number, yytext);
  enginx_compile_error_create(400, msg_buffer);
  return 0;
}

#define STRING_ALLOC_SIZE (256)

static char* ex_string_literal_buffer = NULL;
static int ex_string_literal_buffer_size = 0;
static int ex_string_literal_buffer_alloc_size = 0;

void enginx_begin_string_literal()
{
  ex_string_literal_buffer_size = 0;
}
void enginx_add_string_literal(int letter)
{
  if (ex_string_literal_buffer_size == ex_string_literal_buffer_alloc_size) {
    ex_string_literal_buffer_alloc_size += STRING_ALLOC_SIZE;
    ex_string_literal_buffer = MEM_realloc(ex_string_literal_buffer, ex_string_literal_buffer_alloc_size);
  }
  ex_string_literal_buffer[ex_string_literal_buffer_size] = letter;
  ex_string_literal_buffer_size++;
}

char* enginx_close_string_literal()
{
  char* new_str;
  new_str = enginx_malloc(ex_string_literal_buffer_size + 1);
  memcpy(new_str, ex_string_literal_buffer, ex_string_literal_buffer_size);
  new_str[ex_string_literal_buffer_size] = '\0';
  return new_str;
}

void enginx_reset_string_literal()
{
  MEM_free(ex_string_literal_buffer);
  ex_string_literal_buffer = NULL;
  ex_string_literal_buffer_size = 0;
  ex_string_literal_buffer_alloc_size = 0;
}

//debug methods
void debug_print_current_servers()
{
#ifdef DEBUG
  ENGINX_INTERPRETER* inter = enginx_get_current_interpreter();
  if (inter == NULL) {
    printf("current interpreter is empty");
    return;
  }
  ENGINX_SERVER* pos_server;
  for (pos_server = inter->server_list; pos_server; pos_server = pos_server->next) {
    printf("=========== begin server =============\n");
    printf("server domain: %s\n", pos_server->domain->u.string_value);
    printf("server scheme: %s\n", pos_server->scheme->u.string_value);
    printf("server port: %d\n", pos_server->port);
    debug_print_server_locations(pos_server);
    printf("=========== end server ==============\n");
  }
#endif
}

void debug_print_server_locations(ENGINX_SERVER* server)
{
#ifdef DEBUG
  if (!server) {
    return;
  }
  ENGINX_LOCATION* pos;
  for (pos = server->locations; pos; pos = pos->next) {
    printf("----------- begin location -----------\n");
    printf("location matcher: %s\n", pos->matcher->u.string_value);
    ENGINX_STATEMENT_LIST* st_pos;
    for (st_pos = pos->location_statements; st_pos; st_pos = st_pos->next) {
      printf("location statement: %d\n", st_pos->statement->type);
    }
    printf("----------- end location -----------\n");
  }
#endif
}

unsigned char enginx_to_hex(unsigned char x)
{
  return x > 9 ? x + 55 : x + 48;
}

unsigned char enginx_from_hex(unsigned char x)
{
  unsigned char y;
  if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
  else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
  else if (x >= '0' && x <= '9') y = x - '0';
  else y = '0';
  return y;
}

char* enginx_url_encode(char* str)
{
  unsigned int index = 0;
  size_t length = strlen(str);
  char* result = (char*)malloc(512);
  for (int i = 0; i < length; i++) {
    if (isalnum((unsigned char)str[i]) ||
        (str[i] == '-') ||
        (str[i] == '_') ||
        (str[i] == '.') ||
        (str[i] == '~')) {
      result[index++] = str[i];
    } else if (str[i] == ' ') {
      result[index++] = '+';
    } else {
      result[index++] = '%';
      result[index++] = enginx_to_hex((unsigned char)str[i] >> 4);
      result[index++] = enginx_to_hex((unsigned char)str[i] % 16);
    }
  }
  result[index] = '\0';
  return result;
}
char* enginx_url_decode(char* str)
{
  unsigned int index = 0;
  size_t length = strlen(str);
  char* result = (char*)malloc(512);
  for (int i = 0; i < length; i++) {
    if (str[i] == '+') {
      result[index++] = ' ';
    } else if (str[i] == '%') {
      if (i + 2 >= length) {
        break;
      }
      unsigned char high = enginx_from_hex((unsigned char)str[++i]);
      unsigned char low = enginx_from_hex((unsigned char)str[++i]);
      result[index++] = high*16 + low;
    } else {
      result[index++] = str[i];
    }
  }
  result[index] = '\0';
  return result;
}
