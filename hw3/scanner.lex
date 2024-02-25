%{
    /* Declarations section */
#include <stdio.h>
#include "types.hpp"
#include "hw3_output.hpp"
#include "parser.tab.hpp"
%}

%option yylineno
%option noyywrap
digit   		    ([0-9])
letter  		    ([a-zA-Z])
whitespace		    ([\t\n\r ])
sc                  (;)
comma               (,)
lparen              (\()
rparen              (\))
lbrace              (\{)
rbrace              (\})
assign              (\=)
relop               (<|>|<=|>=)
id                  ({letter}[a-zA-Z0-9]*)
num                 ([1-9]{digit}*|0)
string              \"([^\n\r\"\\]|\\[rnt"\\])+\"
ignore              \/\/[^\r\n]*[\r|\n|\r\n]?


%%
void        return VOID;
int         return INT;
byte        return BYTE;
b           return B;
bool        return BOOL;
override    return OVERRIDE;
and         return AND;
or          return OR;
not         return NOT;
true        return TRUE;
false       return FALSE;
return      return RETURN;
if          return IF;
else        return ELSE;
while       return WHILE;
break       return BREAK;
continue    return CONTINUE;
{sc}        return SC;
{comma}     return COMMA;
{lparen}    return LPAREN;
{rparen}    return RPAREN;
{lbrace}    return LBRACE;
{rbrace}    return RBRACE;
{assign}    return ASSIGN;
{relop}     return RELATIONAL_OP;
(==|!=)     return EQUALITY_OP;
([*/])      return MULTIPLICATIVE_BINOP;
([\-+])     return ADDITIVE_BINOP;
{id}        {yylval = new Name_attr(yytext);  return ID;}
{num}       {yylval = new Number_attr(yytext); return NUM;}
{string}    return STRING;
{ignore}     ;
{whitespace} ;
.           {output::errorLex(yylineno); exit(0);}
%%  