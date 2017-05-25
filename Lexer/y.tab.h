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
     SERVER = 259,
     DOMAIN = 260,
     PORT = 261,
     LOCATION = 262,
     LP = 263,
     RP = 264,
     LC = 265,
     RC = 266,
     SEMICOLON = 267,
     COLON = 268,
     IF = 269,
     ENCODE = 270,
     DECODE = 271,
     RETURN = 272,
     MATCH = 273,
     PARSE = 274,
     DEFINE = 275,
     GREATER = 276,
     EQUAL = 277,
     SMALLER = 278,
     SCHEME = 279
   };
#endif
/* Tokens.  */
#define STRING_LITERAL 258
#define SERVER 259
#define DOMAIN 260
#define PORT 261
#define LOCATION 262
#define LP 263
#define RP 264
#define LC 265
#define RC 266
#define SEMICOLON 267
#define COLON 268
#define IF 269
#define ENCODE 270
#define DECODE 271
#define RETURN 272
#define MATCH 273
#define PARSE 274
#define DEFINE 275
#define GREATER 276
#define EQUAL 277
#define SMALLER 278
#define SCHEME 279




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 7 "enginx.y"
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
#line 109 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

