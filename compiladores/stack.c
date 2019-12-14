#include <stdlib.h> // for malloc
#include <string.h>
#include "stack.h" // STACK header

Instr* stack_Instr1(Ikind kind,int arg){
  Instr* node = (Instr*) malloc(sizeof(Instr*));
  node->kind = kind;
  node->attr.arg = arg;
  node->arg_kind = val;
  return node;
}

Instr* stack_Instr2(Ikind kind,char* id){
  Instr* node = (Instr*) malloc(sizeof(Instr*));
  node->kind = kind;
  node->attr.id = id;
  node->arg_kind = var;
  return node;
}

Instr* stack_Instr3(Ikind kind){
  Instr* node = (Instr*) malloc(sizeof(Instr*));
  node->kind = kind;
  node->arg_kind = none;
  return node;
}


Instr* head(InstrList* l){ return l->code;};

InstrList* tail(InstrList* l){ return l->next;};

InstrList* append(InstrList* l1, InstrList* l2){
  if(l1==NULL)
    return l2;
  else{
    return stack_InstrList(head(l1),append(tail(l1),l2));
  }
}

InstrList* stack_InstrList(Instr* code,InstrList* next){
  InstrList* node = (InstrList*) malloc(sizeof(InstrList*));
  node->code = code;
  node->next = next;
  return node;
}

varList* stack_varList(char* var, varList* next){
  varList* node = (varList*) malloc(sizeof(varList*));
  node->var = var;
  node->next = next;
  return node;
}

int varListContains(char* var,varList* varlist){
  varList* l = varlist;
  while(l != NULL){
    if(strcmp(l->var,var) == 0){
      return 1;
    }
    else{
      l = l->next;
    }
  }
  return 0;
}

char* head2(varList* v){ return v->var;};

varList* tail2(varList* v){return v->next;};

varList* append2(varList* l1, varList* l2){
  if(l1==NULL)
    return l2;
  else{
    return stack_varList(head2(l1),append2(tail2(l1),l2));
  }
}
