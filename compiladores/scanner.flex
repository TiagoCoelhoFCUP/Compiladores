%{
// HEADERS
#include <stdlib.h>
#include "parser.h"
#include <string.h>

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%
[ \t]+ {  }
#.*\n { yyline++; }
\n { yyline++; }

\-?[0-9]+ {
   yylval.intValue = atoi(yytext);
   return INT;
}
"+"         { return PLUS; }
"-"         { return LESS; }
"*"         { return MULT; }
"/"         { return DIV;  }
"%"         { return REST; }
"=="        { return EQUAL; }
"&&"        { return AND; }
"||"        { return OR; }
"!="        { return NOTEQUAL; }
">="        { return GREATER_OR_EQUAL; }
"<="        { return LESSER_OR_EQUAL; }
">"         { return GREATER; }
"<"         { return LESSER; }
"="         { return ATRIB; }
"while"     { return WHILE; }
"if"        { return IF; }
"else"      { return ELSE; }
"{"         { return OPEN_BRACKETS; }
"}"         { return CLOSE_BRACKETS; }
"func" 	    { return FUNC; }
"main"      { return MAIN; }
"("         { return OPEN_PARENTESIS; }
")"         { return CLOSE_PARENTESIS; }
";"         { return SEMICOLON; }
"int"       { return INT_TYPE; }
"%d"        { return INT_OP;}
","         { return COMMA;}
["]         { return QUOTATION;}
"printf"    { return PRINTF;}
"scanf"     { return SCANF;}

[a-z][a-z0-9]* {
   yylval.id = strdup(yytext);
   return VAR;
}


.  { yyerror("unexpectede character"); }
%%
