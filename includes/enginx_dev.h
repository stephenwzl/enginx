//
//  Debug.h
//  xcode
//
//  Created by stephenw on 2017/5/16.
//  Copyright © 2017年 stephenw.cc. All rights reserved.
//

#ifndef enginx_dev_h
#define enginx_dev_h

#include <stdio.h>
#include "Mem.h"

typedef char*                                   ENGINX_STRING;
typedef struct enginx_value_node                ENGINX_VALUE;
typedef struct enginx_location_node             ENGINX_LOCATION;
typedef struct enginx_expression_node           ENGINX_EXPRESSION;
typedef struct enginx_statement_node            ENGINX_STATEMENT;
typedef struct enginx_statement_list_node       ENGINX_STATEMENT_LIST;
typedef struct enginx_interpreter_node          ENGINX_INTERPRETER;

typedef enum {
  ENGINX_FALSE = 0,
  ENGINX_TRUE = 1
} ENGINX_BOOLEAN;

typedef enum {
  ENGINX_BOOLEAN_VALUE = 1,
  ENGINX_STRING_VALUE,
  ENGINX_INT_VALUE,
  ENGINX_NULL_VALUE,
  ENGINX_IDENTIFIER_VALUE
} ENGINX_VALUE_TYPE;

struct enginx_value_node {
  ENGINX_VALUE_TYPE type;
  union {
    ENGINX_BOOLEAN  boolean_value;
    ENGINX_STRING   string_value;
    ENGINX_STRING   identifier_value;
    int             int_value;
  } u;
};

//defines the server model
typedef struct enginx_server_node {
  ENGINX_VALUE*               domain;
  ENGINX_VALUE*               scheme;
  unsigned int                port;
  ENGINX_LOCATION*            locations;
  struct enginx_server_node*  next;
} ENGINX_SERVER;

//defines the location model
struct enginx_location_node {
  ENGINX_VALUE*                 matcher;
  ENGINX_STATEMENT_LIST*        location_statements;
  struct enginx_location_node*  next;
};


typedef enum {
  ENCODE_EXPRESSION = 1,
  DECODE_EXPRESSION,
  MATCH_EXPRESSION,
  RETURN_EXPRESSION,
  DEFINE_EXPRESSION,
  EQUAL_EXPRESSION,
  GREATER_EXPRESSION,
  SMALLER_EXPRESSION,
  PARSE_EXPRESSION,
  UNKNOWN_EXPRESSION
} ENGINX_EXPRESSION_TYPE;

//
typedef struct enginx_argumentlist_node {
  ENGINX_VALUE*               value;
  struct enginx_argumentlist_node*   next;
} ENGINX_ARGUMENT_LIST;

//prototype of single line expression
struct enginx_expression_node {
  ENGINX_EXPRESSION_TYPE  type;
  int                     line_number;
  ENGINX_ARGUMENT_LIST*   list;
};

typedef enum {
  EXPRESSION_STATEMENT = 1,
  IF_STATEMENT,
  UNKNOWN_STATEMENT
} ENGINX_STATEMENT_TYPE;

typedef struct {
  ENGINX_STATEMENT_LIST* statement_list;
} ENGINX_BLOCK;

typedef struct {
  ENGINX_EXPRESSION*  condition;
  ENGINX_BLOCK*       then_block;
} ENGINX_IF_STATMENT;

//prototype of single statement
struct enginx_statement_node {
  ENGINX_STATEMENT_TYPE type;
  int                   line_number;
  union {
    ENGINX_EXPRESSION*  expression;
    ENGINX_IF_STATMENT* if_statement;
  } u;
};

//prototype of statement list
struct enginx_statement_list_node {
  ENGINX_STATEMENT* statement;
  struct enginx_statement_list_node* next;
};

struct enginx_interpreter_node {
  MEM_Storage     interpreter_storage;
  MEM_Storage     execute_storage;
  ENGINX_SERVER   *server_list;
  int             current_line_number;
};


//function definitions
//interpreters
void enginx_set_current_interpreter(ENGINX_INTERPRETER* inter);
ENGINX_INTERPRETER* enginx_get_current_interpreter();

//enginx memory
void* enginx_malloc(size_t size);

//creat entries
ENGINX_INTERPRETER* enginx_create_interpreter();
int enginx_compile_file(ENGINX_INTERPRETER* interpreter, FILE *fp);
int enginx_compile_string(ENGINX_INTERPRETER* interpreter, char* str);
void enginx_reset_interpreter(ENGINX_INTERPRETER *interpreter);

//each element creators
ENGINX_VALUE* enginx_create_string_value(char* str);
ENGINX_VALUE* enginx_create_int_value(int value);
ENGINX_VALUE* enginx_create_identifier_value(char* str);
ENGINX_VALUE* enginx_create_null_value();
ENGINX_EXPRESSION* enginx_create_expression(ENGINX_EXPRESSION_TYPE type, ENGINX_ARGUMENT_LIST* argument_list);
ENGINX_ARGUMENT_LIST* enginx_create_argument_list(ENGINX_VALUE* value);
ENGINX_ARGUMENT_LIST* enginx_chain_argument_list(ENGINX_ARGUMENT_LIST* alist, ENGINX_VALUE* value);

ENGINX_STATEMENT* enginx_create_normal_statement(ENGINX_EXPRESSION* expression);
ENGINX_STATEMENT* enginx_create_if_statement(ENGINX_EXPRESSION* condition, ENGINX_BLOCK* block);
ENGINX_STATEMENT_LIST* enginx_create_statement_list(ENGINX_STATEMENT* statement);
ENGINX_STATEMENT_LIST* enginx_chain_statement_list(ENGINX_STATEMENT_LIST* list, ENGINX_STATEMENT* statement);
ENGINX_BLOCK* enginx_create_block(ENGINX_STATEMENT_LIST* list);

ENGINX_LOCATION* enginx_create_location(ENGINX_VALUE* matcher, ENGINX_STATEMENT_LIST* statement_list);
ENGINX_LOCATION* enginx_chain_location(ENGINX_LOCATION* list, ENGINX_LOCATION* location);

ENGINX_SERVER* enginx_create_server(ENGINX_VALUE* domain, ENGINX_VALUE* scheme, unsigned int port, ENGINX_LOCATION* locations);
ENGINX_SERVER* enginx_chain_server(ENGINX_SERVER* list, ENGINX_SERVER* server);

char* enginx_create_string(char* str);
int enginx_value_to_integer(ENGINX_VALUE* value);

//strings
void enginx_begin_string_literal();
void enginx_add_string_literal(int letter);
char* enginx_close_string_literal();
void enginx_reset_string_literal();

//encode methods
unsigned char enginx_to_hex(unsigned char x);
unsigned char enginx_from_hex(unsigned char x);
//manage return str memory yourself
char* enginx_url_encode(char* str);
char* enginx_url_decode(char* str);

#endif
