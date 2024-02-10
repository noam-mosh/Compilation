%{
/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"
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
quotation               (\")
hexchar                 ([0-9a-fA-F])
string                  ([\x00-\x09\x0b-\x0c\x0e-\x21\x23-\x5b\x5d-\x7f])
escape                  (\\n|\\r|\\t|\\0|\\x{hexchar}{2}|\\.)



%x STRING_STATE
%%

void                                                                            return VOID;
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
([=!<>]=)|([<>])                                                                return RELOP;
[\+\-\*\/]                                                                      return BINOP;
(\/\/)[^\n\r]*                                                                  return COMMENT;
{letter}({letter}|{digit})*                                                     return ID;
([1-9]({digit})*)|0                                                             return NUM;
{whitespace}				                                                    ;
{quotation}                                                                     BEGIN(STRING_STATE);
<STRING_STATE>({string}|{escape})*{quotation}                                   BEGIN(INITIAL); return STRING;
<STRING_STATE>.|[\r\n]                                                          return UNCLOSED_STRING;
<STRING_STATE><<EOF>>                                                           return UNCLOSED_STRING;
.                                                                               return UNDEFINED_CHAR;
%%
