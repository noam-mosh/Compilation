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
whitespace		        ([ {HT}{CR}{LF}])
quotation               (\")
hexchar                 ([0-9a-fA-F])
escape                  ({quotation}|{LF}|{CR}|{HT}|\0|\x{hexchar}{2})
printable               ([^\{quotation}{LF}{CR}])



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
(\/\/)[^{LF}{CR}]*                                                              return COMMENT;
{letter}({letter}|{digit})*                                                     return ID;
([1-9]({digit})*)|0                                                             return NUM;
{whitespace}				                                                    ;
{quotation}                                                                     BEGIN(STRING_STATE);
<STRING_STATE>({printable}|\\({escape}))*{quotation}                            BEGIN(INITIAL);
                                                                                return STRING;
<STRING_STATE>\\([^{escape}])*{quotation}                                       return UNDEFINED_ESCAPE_SEQUENCE;
<STRING_STATE>([^{quotation}]*)                                                 return UNCLOSED_STRING;
.                                                                               return UNDEFINED_CHAR;
%%
