// AST constructor functions

#include <stdlib.h> // for malloc
#include "ast.h" // AST header

Expr* ast_integer(int v) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_INTEGER;
  node->attr.value = v;
  return node;
}

Expr* ast_operation(int operator, Expr* left, Expr* right) {
  Expr* node = (Expr*) malloc(sizeof(Expr));
  node->kind = E_OPERATION;
  node->attr.op.operator = operator;
  node->attr.op.left = left;
  node->attr.op.right = right;
  return node;
}


Expr* ast_id (char* id) {
  Expr* node = malloc(sizeof(Expr));
  node->kind = E_ID;
  node->attr.id = id;
  return node;
}


Cmd* ast_atribution(char* id, Expr* expr){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_ASG;
  node->attr._asg.id = id;
  node->attr._asg.expr = expr;
  return node;
}

Cmd* ast_while(Expr* expr, CmdList* cmds) {
  Cmd* node = (Cmd*) malloc(sizeof(Cmd*));
  node->kind = C_WHILE;
  node->attr._while.expr = expr;
  node->attr._while.cmds = cmds;
  return node;
}

Cmd* ast_if(Expr* expr, CmdList* cmds) {
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_IF;
  node->attr._if.expr = expr;
  node->attr._if.cmds = cmds;
  return node;
}

Cmd* ast_if_else(Expr* expr, CmdList* if_part, CmdList* else_part) {
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_IF_ELSE;
  node->attr._if_else.expr = expr;
  node->attr._if_else.if_part = if_part;
  node->attr._if_else.else_part = else_part;
  return node;
}

CmdList* ast_cmdlist(Cmd* cmd, CmdList* next){
  CmdList* node = (CmdList*) malloc(sizeof(CmdList));
  node->cmd = cmd;
  node->next = next;
  return node;
}

Cmd* ast_scan(char* id){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_SCAN;
  node->attr._scan.id = id;
  return node;
}

Cmd* ast_print(char* id){
  Cmd* node = (Cmd*) malloc(sizeof(Cmd));
  node->kind = C_PRINT;
  node->attr._print.id = id;
  return node;
}
