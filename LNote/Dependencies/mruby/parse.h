/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
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


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     keyword_class = 258,
     keyword_module = 259,
     keyword_def = 260,
     keyword_undef = 261,
     keyword_begin = 262,
     keyword_rescue = 263,
     keyword_ensure = 264,
     keyword_end = 265,
     keyword_if = 266,
     keyword_unless = 267,
     keyword_then = 268,
     keyword_elsif = 269,
     keyword_else = 270,
     keyword_case = 271,
     keyword_when = 272,
     keyword_while = 273,
     keyword_until = 274,
     keyword_for = 275,
     keyword_break = 276,
     keyword_next = 277,
     keyword_redo = 278,
     keyword_retry = 279,
     keyword_in = 280,
     keyword_do = 281,
     keyword_do_cond = 282,
     keyword_do_block = 283,
     keyword_do_LAMBDA = 284,
     keyword_return = 285,
     keyword_yield = 286,
     keyword_super = 287,
     keyword_self = 288,
     keyword_nil = 289,
     keyword_true = 290,
     keyword_false = 291,
     keyword_and = 292,
     keyword_or = 293,
     keyword_not = 294,
     modifier_if = 295,
     modifier_unless = 296,
     modifier_while = 297,
     modifier_until = 298,
     modifier_rescue = 299,
     keyword_alias = 300,
     keyword_BEGIN = 301,
     keyword_END = 302,
     keyword__LINE__ = 303,
     keyword__FILE__ = 304,
     keyword__ENCODING__ = 305,
     tIDENTIFIER = 306,
     tFID = 307,
     tGVAR = 308,
     tIVAR = 309,
     tCONSTANT = 310,
     tCVAR = 311,
     tLABEL = 312,
     tINTEGER = 313,
     tFLOAT = 314,
     tCHAR = 315,
     tREGEXP = 316,
     tSTRING = 317,
     tSTRING_PART = 318,
     tNTH_REF = 319,
     tBACK_REF = 320,
     tREGEXP_END = 321,
     tUPLUS = 322,
     tUMINUS = 323,
     tPOW = 324,
     tCMP = 325,
     tEQ = 326,
     tEQQ = 327,
     tNEQ = 328,
     tGEQ = 329,
     tLEQ = 330,
     tANDOP = 331,
     tOROP = 332,
     tMATCH = 333,
     tNMATCH = 334,
     tDOT2 = 335,
     tDOT3 = 336,
     tAREF = 337,
     tASET = 338,
     tLSHFT = 339,
     tRSHFT = 340,
     tCOLON2 = 341,
     tCOLON3 = 342,
     tOP_ASGN = 343,
     tASSOC = 344,
     tLPAREN = 345,
     tLPAREN_ARG = 346,
     tRPAREN = 347,
     tLBRACK = 348,
     tLBRACE = 349,
     tLBRACE_ARG = 350,
     tSTAR = 351,
     tAMPER = 352,
     tLAMBDA = 353,
     tSYMBEG = 354,
     tREGEXP_BEG = 355,
     tWORDS_BEG = 356,
     tQWORDS_BEG = 357,
     tSTRING_BEG = 358,
     tSTRING_DVAR = 359,
     tLAMBEG = 360,
     tLOWEST = 361,
     tUMINUS_NUM = 362,
     idNULL = 363,
     idRespond_to = 364,
     idIFUNC = 365,
     idCFUNC = 366,
     id_core_set_method_alias = 367,
     id_core_set_variable_alias = 368,
     id_core_undef_method = 369,
     id_core_define_method = 370,
     id_core_define_singleton_method = 371,
     id_core_set_postexe = 372,
     tLAST_TOKEN = 373
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1685 of yacc.c  */
#line 838 "parse.y"

    node *nd;
    mrb_sym id;
    int num;
    unsigned int stack;
    const struct vtable *vars;



/* Line 1685 of yacc.c  */
#line 179 "D:/Programing/mruby/mruby-mruby-d416a5c/build/src/parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




