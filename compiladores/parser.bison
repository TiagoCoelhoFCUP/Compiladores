%token
  INT
  PLUS
  LESS
  MULT
  DIV
  REST
  EQUAL
  AND
  OR
  NOTEQUAL
  GREATER_OR_EQUAL
  LESSER_OR_EQUAL
  GREATER
  LESSER
  ATRIB
  INT_TYPE
  INT_OP
  WHILE
  IF
  ELSE
  OPEN_BRACKETS
  CLOSE_BRACKETS
  OPEN_PARENTESIS
  CLOSE_PARENTESIS
  SEMICOLON
  VAR
  FUNC
  MAIN
  QUOTATION
  COMMA
  PRINTF
  SCANF

// Operator associativity & precedence
%right ATRIB
%left AND OR
%left EQUAL NOTEQUAL
%left GREATER LESSER GREATER_OR_EQUAL LESSER_OR_EQUAL
%left PLUS LESS
%left MULT DIV
%left REST

// Root-level grammar symbol
%start program;

// Types/values in association to grammar symbols.
%union {
  int intValue;
  char* id;
  Expr* exprValue;
  CmdList* cmdListValue;
  Cmd* cmdValue;
}
%type <intValue> INT
%type <id> VAR
%type <exprValue> expr
%type <cmdListValue> cmdlist
%type <cmdValue> cmd

// Use "%code requires" to make declarations go
// into both parser.c and parser.h
%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

extern int yylex();
extern int yyline;
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
CmdList* root;
}

%define parse.error verbose

%%
program: FUNC MAIN OPEN_PARENTESIS CLOSE_PARENTESIS OPEN_BRACKETS cmdlist CLOSE_BRACKETS { root = $6; }

cmdlist:
 { $$ = NULL; }
 |
 cmd cmdlist { $$ = ast_cmdlist($1, $2); root=$$; }
 ;


cmd:
 INT_TYPE VAR ATRIB expr SEMICOLON {
     $$ = ast_atribution($2,$4);
 }
 |
 VAR ATRIB expr SEMICOLON {
     $$ = ast_atribution($1,$3);
 }
 |
 IF OPEN_PARENTESIS expr CLOSE_PARENTESIS OPEN_BRACKETS cmdlist CLOSE_BRACKETS {
     $$ = ast_if($3, $6);
 }
 |
 IF OPEN_PARENTESIS expr CLOSE_PARENTESIS OPEN_BRACKETS cmdlist CLOSE_BRACKETS ELSE OPEN_BRACKETS cmdlist CLOSE_BRACKETS {
     $$ = ast_if_else($3 , $6, $10);
 }
 |
 WHILE OPEN_PARENTESIS expr CLOSE_PARENTESIS OPEN_BRACKETS cmdlist CLOSE_BRACKETS {
     $$ = ast_while($3, $6);
 }
 |
 PRINTF OPEN_PARENTESIS QUOTATION INT_OP QUOTATION COMMA VAR CLOSE_PARENTESIS SEMICOLON {
    $$ = ast_print($7);
 }
 |
 SCANF OPEN_PARENTESIS QUOTATION INT_OP QUOTATION COMMA VAR CLOSE_PARENTESIS SEMICOLON {
    $$ = ast_scan($7);
 }
 ;
expr:
  INT {
    $$ = ast_integer($1);
  }
  |
  VAR {
    $$ = ast_id($1);
  }
  |
  expr PLUS expr {
    $$ = ast_operation(PLUS, $1, $3);
  }
  |
  expr LESS expr {
	$$ = ast_operation(LESS, $1, $3);
  }
  |
  expr DIV expr {
	$$ = ast_operation(DIV, $1, $3);
  }
  |
  expr REST expr {
	$$ = ast_operation(REST, $1, $3);
  }
  |
  expr MULT expr {
	$$ = ast_operation(MULT, $1, $3);
  }
  |
  expr EQUAL expr {
       $$ = ast_operation(EQUAL, $1, $3);
  }
  |
  expr NOTEQUAL expr {
       $$ = ast_operation(NOTEQUAL, $1, $3);
  }
  |
  expr AND expr {
       $$ = ast_operation(AND, $1, $3);
  }
  |
  expr OR expr {
       $$ = ast_operation(OR, $1, $3);
  }
  |
  expr GREATER_OR_EQUAL expr {
       $$ = ast_operation(GREATER_OR_EQUAL, $1, $3);
  }
  |
  expr LESSER_OR_EQUAL expr {
       $$ = ast_operation(LESSER_OR_EQUAL, $1, $3);
  }
  |
  expr GREATER expr {
       $$ = ast_operation(GREATER, $1, $3);
  }
  |
  expr LESSER expr {
       $$ = ast_operation(LESSER, $1, $3);
  }
  ;

%%
void yyerror(const char* err) {
  printf("Line %d: %s - '%s'\n", yyline, err, yytext  );
}
