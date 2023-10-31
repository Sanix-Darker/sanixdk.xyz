/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_JINJA_EXPRESSION_TAB_H_INCLUDED
# define YY_YY_JINJA_EXPRESSION_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STRING_LITERAL = 258,
    IDENTIFIER = 259,
    FLOAT = 260,
    INTEGER = 261,
    FOR = 262,
    END_FOR = 263,
    IN = 264,
    IF = 265,
    IS = 266,
    ELSE = 267,
    END_IF = 268,
    ELIF = 269,
    L_TRUE = 270,
    L_FALSE = 271,
    BLOCK = 272,
    END_BLOCK = 273,
    EXTENDS = 274,
    RAW = 275,
    END_RAW = 276,
    SET = 277,
    EQUAL = 278,
    HIGH_AND_EQUAL_THAN = 279,
    LOWER_AND_EQUAL_THAN = 280,
    DIFFERENT = 281,
    AND = 282,
    OR = 283,
    NOT = 284,
    LOWER_THAN = 285,
    HIGHER_THAN = 286
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 51 "jinja_expression.y"

  int32_t integerData;
  double doubleData;
  char *stringData;

#line 95 "jinja_expression.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_JINJA_EXPRESSION_TAB_H_INCLUDED  */
