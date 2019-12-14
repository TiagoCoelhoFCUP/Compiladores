
// AST definitions
#ifndef __ast_h__
#define __ast_h__

// AST for expressions
struct _Expr {
  enum {
    E_INTEGER,
    E_OPERATION,
    E_ID,
  } kind;
  union {
    int value; // for integer values
    char* id;
    struct {
      int operator; // PLUS, MINUS, etc
      struct _Expr* left;
      struct _Expr* right;
    } op; // for binary expressions
  } attr;
};


//AST for commands
struct _Cmd {
  enum {
    C_ASG,
    C_WHILE,
    C_IF,
    C_IF_ELSE,
    C_PRINT,
    C_SCAN
  }kind;
  union {

    struct {
      char* id;
      struct _Expr* expr;
    } _asg;

    struct {
      struct _Expr* expr;
      struct _CmdList* cmds;
    } _while;

    struct {
      struct _Expr* expr;
      struct _CmdList* cmds;
    }_if;

    struct {
      struct _Expr* expr;
      struct _CmdList* if_part;
      struct _CmdList* else_part;
    }_if_else;

    struct {
      char* id;
    }_scan;

    struct{
      char* id;
    }_print;

  } attr;
};

struct _CmdList {
  struct _Cmd* cmd;
  struct _CmdList* next;
};


typedef struct _Expr Expr;
typedef struct _BoolExpr BoolExpr;
typedef struct _Cmd Cmd;
typedef struct _CmdList CmdList;

// Constructor functions (see implementation in ast.c)

Expr* ast_integer(int v);
Expr* ast_operation(int operator, Expr* left, Expr* right);
Expr* ast_id (char* id);

Cmd* ast_if(Expr* expr, CmdList* cmd);
Cmd* ast_if_else(Expr* expr, CmdList* if_part, CmdList* else_part);
Cmd* ast_while(Expr* expr, CmdList* cmds);
Cmd* ast_atribution(char* id, Expr* expr);
Cmd* ast_scan(char* id);
Cmd* ast_print(char* id);

CmdList* ast_cmdlist(Cmd* cmd, CmdList* next);


#endif
