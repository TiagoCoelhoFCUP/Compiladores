#include <stdio.h>
#include "parser.h"
#include "ast.h"

void printCmd(Cmd* cmd,int spaces);
void printCmdList(CmdList* cmdList,int spaces);
void printExpr(Expr* expr,int spaces);
void printSpaces(int spaces);


int main(int argc, char** argv) {
  --argc; ++argv;
  if (argc != 0) {
    yyin = fopen(*argv, "r");
    if (!yyin) {
      printf("'%s': could not open file\n", *argv);
      return 1;
    }
  } //  yyin = stdin
  if (yyparse() == 0) { //parsing was successfull
    printf("Func Main() {\n");
    while(root!=NULL){
      printCmd(root->cmd,2);
      root = root->next;
    }
  }
  printf("}\n");
  return 0;
}

//Print Cmdlist
void printCmdList(CmdList* cmdList,int spaces) {
  CmdList* current = cmdList;
  while(current!=NULL) {
    printCmd(current->cmd,spaces);
    current = current->next;
  }
}

//print Cmd
void printCmd(Cmd* cmd,int spaces) {
  if(cmd->kind == C_IF) {
    printSpaces(spaces);
    printf("IF (\n");
    printExpr(cmd->attr._if.expr,spaces+2);
    printSpaces(spaces);
    printf(") {\n");
    printCmdList(cmd->attr._if.cmds,spaces+2);
    printSpaces(spaces);
    printf("}\n");
  }
  else if(cmd->kind == C_IF_ELSE) {
    printSpaces(spaces);
    printf("IF (\n");
    printExpr(cmd->attr._if_else.expr,spaces+2);
    printSpaces(spaces);
    printf(") {\n");
    printCmdList(cmd->attr._if_else.if_part,spaces+2);
    printSpaces(spaces);
    printf("}\n");
    printSpaces(spaces);
    printf("ELSE {\n");
    printCmdList(cmd->attr._if_else.else_part,spaces+2);
    printSpaces(spaces);
    printf("}\n");
  }
  else if(cmd->kind == C_WHILE) {
    printSpaces(spaces);
    printf("WHILE (\n");
    printExpr(cmd->attr._while.expr,spaces+2);
    printSpaces(spaces);
    printf(") {\n");
    printCmdList(cmd->attr._while.cmds,spaces+2);
    printSpaces(spaces);
    printf("}\n");
  }
  else if(cmd->kind == C_ASG){
    printSpaces(spaces);
    printf("=\n");
    printSpaces(spaces+2);
    printf("%s\n",cmd->attr._asg.id);
    printExpr(cmd->attr._asg.expr,spaces+2);
  }
  else if(cmd->kind == C_PRINT){
    printSpaces(spaces);
    printf("PRINTF (\n");
    printSpaces(spaces+2);
    if(cmd->attr._print.op == 1)
      printf("%%d\n");
    else
      printf("%%f\n");
    printSpaces(spaces+2);
    printf("%s\n",cmd->attr._print.id);
    printSpaces(spaces);
    printf(")\n");
  }
  else if(cmd->kind == C_SCAN){
    printSpaces(spaces);
    printf("SCANF (\n");
    printSpaces(spaces+2);
    printf("%%d\n");
    printSpaces(spaces+2);
    printf("%s\n",cmd->attr._print.id);
    printSpaces(spaces);
    printf(")\n");
  }
  if(cmd == 0) {
    yyerror("Null command!\n");
  }

}
//Print expr
void printExpr(Expr* expr,int spaces) {
  if(expr == 0) {
    yyerror("Null expression!\n");
  }
  if(expr->kind == E_INTEGER) {
    printSpaces(spaces);
    printf("%d\n", expr->attr.value);
  }
  else if(expr->kind == E_ID) {
    printSpaces(spaces);
    printf("%s\n", expr->attr.id);
  }
  else if(expr->kind == E_OPERATION) {
    printSpaces(spaces);
    switch(expr->attr.op.operator){
    case PLUS:
      printf("+\n");
      break;
    case LESS:
      printf("-\n");
      break;
    case MULT:
      printf("*\n");
      break;
    case DIV:
      printf("/\n");
      break;
    case REST:
      printf("%%\n");
      break;
    case EQUAL:
      printf("==\n");
      break;
    case AND:
      printf("&&\n");
      break;
    case OR:
      printf("||\n");
      break;
    case NOTEQUAL:
      printf("!=\n");
      break;
    case GREATER_OR_EQUAL:
      printf(">=\n");
      break;
    case LESSER_OR_EQUAL:
      printf("<=\n");
      break;
    case GREATER:
      printf(">\n");
      break;
    case LESSER:
      printf("<\n");
      break;
    default: yyerror("wrong operator");
    }
    printExpr(expr->attr.op.left,spaces+2);
    printExpr(expr->attr.op.right,spaces+2);
  }
}



void printSpaces(int spaces){
  for(int i=0;i<spaces;i++)
    printf(" ");
}
