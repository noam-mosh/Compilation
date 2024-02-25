%{
    /* Declarations section */
#include <stdio.h>
#include "output.hpp"
#include "parser.tab.hpp"
%}

/* Flex config */
%option yylineno
%option noyywrap

/* Macros */
digit   		        ([0-9])
letter  		        ([a-zA-Z])
LF                      \n
CR                      \r
HT                      \t
whitespace		        ([ \t\n\r])
string                  \"([^\n\r\"\\]|\\[rnt"\\])+\"
ignore              \/\/[^\r\n]*[\r|\n|\r\n]?



%x STRING_STATE
%%

int                                                                             return INT;
byte                                                                            return BYTE;
b                                                                               return B;
bool                                                                            return BOOL;
and                                                                             return AND;
or                                                                              return OR;
not                                                                             return NOT;
true                                                                            return TRUE;
false                                                                           return FALSE;
return                                                                          return RETURN;
if                                                                              return IF;
else                                                                            return ELSE;
while                                                                           return WHILE;
break                                                                           return BREAK;
continue                                                                        return CONTINUE;
;                                                                               return SC;
\(                                                                              return LPAREN;
\)                                                                              return RPAREN;
\{                                                                              return LBRACE;
\}                                                                              return RBRACE;
=                                                                               return ASSIGN;
(<=|>=|<|>)                                                                     return RELATIONAL_OP;
(==|!=)                                                                         return EQUALITY_OP;
[\*\/]                                                                          return MULTIPLICATIVE_BINOP;
[\+\-]                                                                          return ADDITIVE_BINOP;
{letter}({letter}|{digit})*                                                     return ID;
([1-9]({digit})*)|0                                                             return NUM;
{whitespace}				                                                    ;
{ignore}                                                                        ;
{string}                                                                        return STRING;
.                                                                               {output::errorLex(yylineno); exit(0);}
%%
  