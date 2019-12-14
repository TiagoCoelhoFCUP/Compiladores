typedef enum { IAND, LDC, ADI, SBI, DVI, MPI, MOD, IOR, LOD, STO ,RDI, FJP, GRT, LES, WRI, UJP, LABEL, EQU, NEQ, GEQ, LEQ} Ikind;

typedef enum { val , var, none} Iarg;

struct _Instr{
  Ikind kind;
  Iarg arg_kind;
  union{
    int arg;
    char* id;
  }attr;
};

struct _InstrList{
  struct _Instr *code;
  struct _InstrList *next;
};

struct _varList{
  char* var;
  struct _varList *next;
};

typedef struct _Instr Instr;
typedef struct _InstrList InstrList;
typedef struct _varList varList;

varList* stack_varList(char* var, varList* next);
int varListContains(char* var,varList* varlist);
char* head2(varList* v);
varList* tail2(varList* v);
varList* append2(varList* l1, varList* l2);
Instr* stack_Instr3(Ikind kind);
Instr* stack_Instr1(Ikind kind,int arg);
Instr* stack_Instr2(Ikind kind,char* id);
Instr* head(InstrList* l);
InstrList* tail(InstrList* l);
InstrList* append(InstrList* l1, InstrList* l2);
InstrList* stack_InstrList(Instr* code,InstrList* next);
