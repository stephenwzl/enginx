/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     STRING_LITERAL = 258,
     IDENTIFIER = 259,
     NULL_VALUE = 260,
     SERVER = 261,
     DOMAIN = 262,
     PORT = 263,
     LOCATION = 264,
     LP = 265,
     RP = 266,
     LC = 267,
     RC = 268,
     SEMICOLON = 269,
     COLON = 270,
     IF = 271,
     ENCODE = 272,
     DECODE = 273,
     RETURN = 274,
     MATCH = 275,
     PARSE = 276,
     DEFINE = 277,
     GREATER = 278,
     EQUAL = 279,
     SMALLER = 280,
     SCHEME = 281
   };
#endif
/* Tokens.  */
#define STRING_LITERAL 258
#define IDENTIFIER 259
#define NULL_VALUE 260
#define SERVER 261
#define DOMAIN 262
#define PORT 263
#define LOCATION 264
#define LP 265
#define RP 266
#define LC 267
#define RC 268
#define SEMICOLON 269
#define COLON 270
#define IF 271
#define ENCODE 272
#define DECODE 273
#define RETURN 274
#define MATCH 275
#define PARSE 276
#define DEFINE 277
#define GREATER 278
#define EQUAL 279
#define SMALLER 280
#define SCHEME 281




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 9 "enginx.y"
{
  ENGINX_VALUE            *argument;
  ENGINX_LOCATION         *locations_list;
  ENGINX_SERVER           *server_list;
  ENGINX_STATEMENT        *statement;
  ENGINX_STATEMENT_LIST   *statement_list;
  ENGINX_IF_STATMENT      *if_statement;
  ENGINX_ARGUMENT_LIST    *argument_list;
  ENGINX_EXPRESSION       *expression;
  ENGINX_BLOCK            *block;
}
/* Line 1529 of yacc.c.  */
#line 113 "enginx_parse.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE enginxlval;

