%{

/* Declarations section */
#include <stdio.h>
#include "tokens.hpp"

%}

%option yylineno
%option noyywrap
digit   		([0-9])
positive        ([1-9])
letter  		([a-zA-Z])
whitespace		([\t\n \r\s])   //\s
legalEscape     (\\[nrt0x\\\"])
illegalEscape    (\\[^nrt0x\\\"])
qu              ([\"])
char            ([^\"\\\n\r])


%x WORK_STRING
%%

void                        return VOID;
int                         return INT;
byte                        return BYTE;
b                           return B;
bool                        return BOOL;
and                         return AND;
or                          return OR;
not                         return NOT;
true                        return TRUE;
false                       return FALSE;
return                      return RETURN;
if                          return IF;
else                        return ELSE;
while                       return WHILE;
break                       return BREAK;
continue                    return CONTINUE;
;                           return SC;
,                           return COMMA;
\(                          return LPAREN;
\)                          return RPAREN;
\{                          return LBRACE;
\}                          return RBRACE;
=                           return ASSIGN;
([<>]=?)|([!=]=)            return RELOP;
[+*/\-]                     return BINOP;
(\/\/)[^\n\r]*              return COMMENT;
({positive}{digit}*)|0      return NUM;
{letter}({digit}|{letter})* return ID;
{qu}                        BEGIN(WORK_STRING);
<WORK_STRING>({char}|{legalEscape})*{qu}    BEGIN(INITIAL); return STRING;
<WORK_STRING>({char}|{illegalEscape}|{char})*{qu}  return UNDEFINED_ESCAPE_SEQUENCE;
<WORK_STRING>.|[\r\n]       return UNCLOSED_STRING;
{whitespace}				;
.                           return UNDEFINED_CHAR;
%%
