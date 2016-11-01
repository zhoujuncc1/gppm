%{
#include <string.h>
#include <stdlib.h>
#include "Bltl.h"
#include "time_variable.h"
#include "parser.tab.hpp"
void yyerror(const char *);
extern "C" int yylex();
%}
%option noyywrap
%%

F       return F;
G       return G;
U       return U;
X       return X;
~       return NOT;
&       return AND;
\|      return OR;
\(		return LPAREN;
\)		return RPAREN;
;		return END;
[a-z][a-zA-Z0-9]+     {yylval.strval = strdup(yytext); return VAR;}
[ \t]+        /* ignore whitespace */
[\n]          /* ignore newline */

\[[a-z0-9_=]+\] {yylval.timval =  new TimeVariable(strdup(yytext)); return TIME;}



%%
