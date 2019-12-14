#include <stdio.h>
#include <string.h>
#include "stack.h"
#include "parser.h"
#include "ast.h"

char * makeLabel();
char * convert_int_string();
void printInstr(Instr* instr);
void printInstrList(InstrList* instrlist);
InstrList* compileExpr(Expr* expr);
InstrList* compileCmd(Cmd* cmd);
InstrList* compileCmdList(CmdList* cmdlist);
void writeInstrList(InstrList* instrlist);
void writeInstr(Instr* instr);


int label_num = 1;
varList* data;

char * convert_int_string(){
  char * str = malloc(sizeof(char)*64);
  sprintf(str,"%d",label_num);
  return str;
}

char * makeLabel(){
  char * str = malloc(sizeof(char)*64);
  char * str2 = convert_int_string(label_num);
  strcpy(str, "Label_");
  strcat(str,str2);
  free(str2);
  label_num++;
  return str;
}

void printInstr(Instr* instr){
  switch(instr->kind){
  case IAND:
    printf("AND ");
    break;
  case LDC:
    printf("LDC ");
    break;
  case ADI:
    printf("ADI ");
    break;
  case SBI:
    printf("SBI ");
    break;
  case DVI:
    printf("DVI ");
    break;
  case MPI:
    printf("MPI ");
    break;
  case MOD:
    printf("MOD ");
    break;
  case IOR:
    printf("IOR ");
    break;
  case LOD:
    printf("LOD ");
    break;
  case STO:
    printf("STO ");
    break;
  case RDI:
    printf("RDI ");
    break;
  case FJP:
    printf("FJP ");
    break;
  case GRT:
    printf("GRT ");
    break;
  case LES:
    printf("LES ");
    break;
  case WRI:
    printf("WRI ");
    break;
  case UJP:
    printf("UJP ");
    break;
  case LABEL:
    printf("LABEL ");
    break;
  case EQU:
    printf("EQU ");
    break;
  case NEQ:
    printf("NEQ ");
    break;
  case GEQ:
    printf("GEQ ");
    break;
  case LEQ:
    printf("LEQ ");
    break;
  default:
    printf("Error, instruction not recognised\n");
    break;
  }
  switch(instr->arg_kind){
    case var:
      printf("%s",instr->attr.id);
      break;
    case val:
      printf("%d",instr->attr.arg);
      break;
    case none:
      break;
    default:
      printf("Error, instruction not recognised\n");
      break;
    }
  printf("\n");
}

void printInstrList(InstrList* instrlist){
  while(instrlist!=NULL){
    printInstr(instrlist->code);
    instrlist = instrlist->next;
  }
}

InstrList* compileExpr(Expr* expr){
  InstrList* l1;
  InstrList* l2;
  if(expr->kind == E_INTEGER)
    return stack_InstrList(stack_Instr1(LDC,expr->attr.value),NULL);
  else if(expr->kind == E_ID)
    return stack_InstrList(stack_Instr2(LOD,expr->attr.id),NULL);
  else{
    switch(expr->attr.op.operator){
    case PLUS:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(ADI),NULL));
      return l2;
    case LESS:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(SBI),NULL));
      return l2;
    case MULT:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(MPI),NULL));
      return l2;
    case DIV:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(DVI),NULL));
      return l2;
    case REST:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(MOD),NULL));
      return l2;
    case AND:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(IAND),NULL));
      return l2;
    case OR:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(IOR),NULL));
      return l2;
    case EQUAL:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(EQU),NULL));
      return l2;
    case NOTEQUAL:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(NEQ),NULL));
      return l2;
    case GREATER_OR_EQUAL:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(GEQ),NULL));
      return l2;
    case LESSER_OR_EQUAL:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(LEQ),NULL));
      return l2;
    case GREATER:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(GRT),NULL));
      return l2;
    case LESSER:
      l1 = append(compileExpr(expr->attr.op.left),compileExpr(expr->attr.op.right));
      l2 = append(l1,stack_InstrList(stack_Instr3(LES),NULL));
      return l2;
    }
  }
}

InstrList* compileCmd(Cmd* cmd){
  InstrList* l1;
  if(cmd->kind == C_ASG){
    if(varListContains(cmd->attr._asg.id,data)==0){
      data = append2(data,stack_varList(cmd->attr._asg.id,NULL));
    }
    l1 = compileExpr(cmd->attr._asg.expr);
    l1 = append(l1,stack_InstrList(stack_Instr2(STO,cmd->attr._asg.id),NULL));
    return l1;
  }
  if(cmd->kind == C_IF){
    l1 = compileExpr(cmd->attr._if.expr);
    char * label = makeLabel();
    l1 = append(l1,stack_InstrList(stack_Instr2(FJP,label),NULL));
    l1 = append(l1,compileCmdList(cmd->attr._if.cmds));
    l1 = append(l1,stack_InstrList(stack_Instr2(LABEL,label),NULL));
    return l1;
  }
  if(cmd->kind == C_IF_ELSE){
    l1 = compileExpr(cmd->attr._if_else.expr);
    char * label = makeLabel();
    char * label2 = makeLabel();
    l1 = append(l1,stack_InstrList(stack_Instr2(FJP,label),NULL));
    l1 = append(l1,compileCmdList(cmd->attr._if_else.if_part));
    l1 = append(l1,stack_InstrList(stack_Instr2(UJP,label2),NULL));
    l1 = append(l1,stack_InstrList(stack_Instr2(LABEL,label),NULL));
    l1 = append(l1,compileCmdList(cmd->attr._if_else.else_part));
    l1 = append(l1,stack_InstrList(stack_Instr2(LABEL,label2),NULL));
    return l1;
  }
  if(cmd->kind == C_WHILE){
    char * label2 = makeLabel();
    char * label = makeLabel();
    l1 = stack_InstrList(stack_Instr2(LABEL,label2),NULL);
    l1 = append(l1,compileExpr(cmd->attr._while.expr));
    l1 = append(l1,stack_InstrList(stack_Instr2(FJP,label),NULL));
    l1 = append(l1,compileCmdList(cmd->attr._while.cmds));
    l1 = append(l1,stack_InstrList(stack_Instr2(UJP,label2),NULL));
    l1 = append(l1,stack_InstrList(stack_Instr2(LABEL,label),NULL));
    return l1;
  }
  if(cmd->kind == C_PRINT){
    l1 = stack_InstrList(stack_Instr2(WRI,cmd->attr._print.id),NULL);
    return l1;
  }
  if(cmd->kind == C_SCAN){
    l1 = stack_InstrList(stack_Instr2(RDI,cmd->attr._scan.id),NULL);
    return l1;
  }
}

InstrList* compileCmdList(CmdList* cmdlist){
  InstrList* l1;
  l1 = compileCmd(cmdlist->cmd);
  while(cmdlist->next != NULL){
    cmdlist = cmdlist->next;
    l1 = append(l1,compileCmd(cmdlist->cmd));
  }
  return l1;
}

void writeInstrList(InstrList* instrlist){
  while(instrlist!=NULL){
    writeInstr(instrlist->code);
    instrlist = instrlist->next;
  }
}

void writeInstr(Instr* instr){
  FILE *fp;
  fp = fopen("temp.txt", "a");
  if(fp==NULL){
    perror("Error opening file.");
  }
  else{
    switch(instr->kind){
    case LDC:
      fprintf(fp, "li $a0,%d\n",instr->attr.arg);
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case ADI:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "add $a0, $a0, $t1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case SBI:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "sub $a0, $a0, $t1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case DVI:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "div $a0, $t1\n");
      fprintf(fp, "mflo $a0\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case MPI:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "mult $a0, $t1\n");
      fprintf(fp, "mflo $a0\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case MOD:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "div $a0, $t1\n");
      fprintf(fp, "mfhi $a0\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case IAND:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "and $a0, $a0, $t1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case IOR:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "or $a0, $a0, $t1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case LOD:
      fprintf(fp, "lw $a0, %s\n",instr->attr.id);
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case STO:
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "sw $a0, %s\n",instr->attr.id);
      break;
    case RDI:
      fprintf(fp, "li $v0, 5\n");
      fprintf(fp, "syscall\n");
      fprintf(fp, "sw $v0, %s\n",instr->attr.id);
      break;
    case WRI:
      fprintf(fp, "lw $a0, %s\n",instr->attr.id);
      fprintf(fp, "li $v0, 1\n");
      fprintf(fp, "syscall\n");
      break;
    case GRT:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "slt $a0, $t1, $a0\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case LES:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "slt $a0, $a0, $t1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case UJP:
      fprintf(fp, "j %s\n",instr->attr.id);
      break;
    case FJP:
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "beq $a0, $zero, %s\n",instr->attr.id);
      break;
    case LABEL:
      fprintf(fp, "%s:\n",instr->attr.id);
      break;
    case EQU:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "subu $a0, $a0, $t1\n");
      fprintf(fp, "sltu $a0, $zero, $a0\n");
      fprintf(fp, "xori $a0, $a0, 1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case NEQ:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $a0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "subu $a0, $a0, $t1\n");
      fprintf(fp, "sltu $a0, $zero, $a0\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case GEQ:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $t0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "slt $a0, $t1, $t0\n");
      fprintf(fp, "subu $a1, $t0, $t1\n");
      fprintf(fp, "sltu $a1, $zero, $a1\n");
      fprintf(fp, "xori $a1, $a1, 1\n");
      fprintf(fp, "or $a0, $a0, $a1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    case LEQ:
      fprintf(fp, "lw $t1, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "lw $t0, 0($sp)\n");
      fprintf(fp, "addiu $sp, $sp, 4\n");
      fprintf(fp, "slt $a0, $t0, $t1\n");
      fprintf(fp, "subu $a1, $t0, $t1\n");
      fprintf(fp, "sltu $a1, $zero, $a1\n");
      fprintf(fp, "xori $a1, $a1, 1\n");
      fprintf(fp, "or $a0, $a0, $a1\n");
      fprintf(fp, "addiu $sp, $sp, -4\n");
      fprintf(fp, "sw $a0, 0($sp)\n");
      break;
    default:
      printf("Error, instruction not recognised\n");
      break;
    }
  }
  fclose(fp);
}

void writeMIPS(){
  char c;
  FILE *fp;
  FILE *fp2;
  remove("mips.txt");
  fp = fopen("temp.txt", "r");
  fp2 = fopen("mips.txt", "a");
  if(fp != NULL && fp2 != NULL){
    fprintf(fp2,".data\n");
    data = data->next;
    while(data!=NULL){
      fprintf(fp2, "%s: .word 4\n",data->var);
      data = data->next;
    }
    fprintf(fp2,"\n");
    fprintf(fp2,".text\n");
    c = fgetc(fp);
    while (c != EOF){
      fputc(c, fp2);
      c = fgetc(fp);
    }
    fprintf(fp2,"li $v0, 10\n");
    fprintf(fp2,"syscall\n");
    fclose(fp);
    fclose(fp2);
    remove("temp.txt");
  }
  else{
    perror("Error opening file.");
  }
}

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
    data = stack_varList("",NULL);
    while(root!=NULL){
      writeInstrList(compileCmd(root->cmd));
      root = root->next;
    }
  }
  writeMIPS();
  return 0;
}
