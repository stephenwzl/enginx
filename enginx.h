#ifndef enginx_h
#define enginx_h

#include <stdio.h>

typedef char* enginx_value;
typedef struct enginx_location_node enginx_location;
typedef struct enginx_expression_node enginx_expression;

//defines the server model
typedef struct enginx_server_node {
  char* name;
  char* scheme;
  enginx_locaton* locations;
  enginx_server_node* next;
} enginx_server;

//defines the location model
struct enginx_location_node {
  char* matcher;
  enginx_location_node* next;
};

typedef struct {
  char* variableName;
  enginx_expression *operand;
}

typedef enum {
  EXPRESSION_STATEMENT = 1,
  ENCODE_STATEMENT,
  DECODE_STATEMENT,
  MATCH_STATEMENT,
  RETURN_STATEMENT,
} enginx_statement_type;

struct enginx_expression_node {
  enginx_statement_type type;
  union {
    enginx_value    value;

  } u;
}


#endif