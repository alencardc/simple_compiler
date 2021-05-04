#include "asm.h"
#include <stdio.h>

AsmInstruction* create_asm_instruction(
  const char* label, 
  const char* opcode, 
  const char* src, 
  const char* dst) {
  AsmInstruction* instruction = (AsmInstruction*) malloc(sizeof(AsmInstruction));

  if (instruction == NULL) {
    return NULL;
  }

  instruction->label = label != NULL ? strdup(label) : NULL;
  instruction->opcode = strdup(opcode);
  instruction->src = src != NULL ? strdup(src) : NULL;
  instruction->dst = dst != NULL ? strdup(dst) : dst;
  instruction->next = NULL;
  instruction->prev = NULL;
  
  return instruction;
}

AsmInstruction* concat_asm_instructions(AsmInstruction* instr1, AsmInstruction* instr2){
  if(instr1 == NULL)
    return instr2;

  if(instr2 == NULL)
    return instr1;

  AsmInstruction* instr1_temp = instr1;
  while(instr1_temp->next != NULL)
    instr1_temp = instr1_temp->next;

  instr1_temp->next = instr2;
  instr2->prev = instr1_temp;
  
  return instr1;
}

AsmInstruction* generate_asm_code(Instruction* iloc_code, Symbol_Entry** global_scope) {
  AsmInstruction* head = NULL;
  AsmInstruction* prev = NULL;

  print_asm_globals_code(global_scope);
  
  while(iloc_code != NULL){
    AsmInstruction* new_asm = iloc_to_asm(iloc_code, prev);
    head = concat_asm_instructions(head, new_asm);

    iloc_code = iloc_code->previous;
    prev = new_asm;
  }
  //print_asm_instructions(head);
  //update_asm_return_addr(head);

  print_asm_instructions(head);
}

AsmInstruction* iloc_to_asm(Instruction* iloc, AsmInstruction* prev){
  if(strcmp(iloc->opcode, "loadI") == 0){
    AsmInstruction* asm_code = create_asm_instruction(NULL, "movl", x86_literal(iloc->operand1), x86_reg(iloc->operand3));
    return asm_code;
  } else if(strcmp(iloc->opcode, "loadAI") == 0){
    AsmInstruction* move = create_asm_instruction(NULL, "movl", x86_offset(iloc->operand1, iloc->operand2), iloc->operand3);
    return move;
  } else if(strcmp(iloc->opcode, "add") == 0){
    AsmInstruction* copy = create_asm_instruction(NULL, "movl", x86_reg(iloc->operand1), x86_reg(iloc->operand3));
    AsmInstruction* add = create_asm_instruction(NULL,"addl" ,x86_reg(iloc->operand2), x86_reg(iloc->operand3));
    concat_asm_instructions(copy, add);
    return copy;
  } else if(strcmp(iloc->opcode, "addI") == 0){
    AsmInstruction* copy = create_asm_instruction(NULL, "movl", iloc->operand1, iloc->operand3);
    AsmInstruction* add = create_asm_instruction(NULL,"addl" , x86_literal(iloc->operand2), iloc->operand3);
    concat_asm_instructions(copy, add);
    return copy;
  } else if(strcmp(iloc->opcode, "sub") == 0){
    AsmInstruction* copy = create_asm_instruction(NULL, "movl", iloc->operand1, iloc->operand3);
    AsmInstruction* sub = create_asm_instruction(NULL,"subl" ,iloc->operand2, iloc->operand3);
    concat_asm_instructions(copy, sub);
    return copy;
  } else if(strcmp(iloc->opcode, "subI") == 0){
    AsmInstruction* copy = create_asm_instruction(NULL, "movl", iloc->operand1, iloc->operand3);
    AsmInstruction* sub = create_asm_instruction(NULL,"subl" ,x86_literal(iloc->operand2), iloc->operand3);
    concat_asm_instructions(copy, sub);
    return copy;
  } else if(strcmp(iloc->opcode, "rsubI") == 0){
    AsmInstruction* copy = create_asm_instruction(NULL, "movl", iloc->operand2, iloc->operand3);
    AsmInstruction* sub = create_asm_instruction(NULL,"subl" ,iloc->operand2, iloc->operand3);
    concat_asm_instructions(copy, sub);
    return copy;
  } else if(strcmp(iloc->opcode, "mult") == 0){
    // Save eax and edx
    AsmInstruction* pushEax = create_asm_instruction(NULL, "pushl", NULL, "%eax");
    AsmInstruction* pushEdx = create_asm_instruction(NULL, "pushl", NULL, "%edx");
    AsmInstruction* movOp1 = create_asm_instruction(NULL, "movl", iloc->operand1, "%eax");
    AsmInstruction* mul = create_asm_instruction(NULL,"imull" ,iloc->operand2, "%eax");
    AsmInstruction* movRes = create_asm_instruction(NULL, "movl", "%eax", iloc->operand3);
    AsmInstruction* popEdx = create_asm_instruction(NULL, "popl", NULL, "%edx");
    AsmInstruction* popEax = create_asm_instruction(NULL, "popl", NULL, "%eax");
    pushEax->next = pushEdx; pushEdx->next = movOp1; movOp1->next = mul;
    mul->next = movRes; movRes->next = popEdx; popEdx->next = popEax;

    popEax->prev = popEdx; popEdx->prev = movRes; movRes->prev = mul; mul->prev = movOp1;
    movOp1->prev = pushEdx; pushEdx->prev = pushEax;
    return pushEax;
  } else if(strcmp(iloc->opcode, "div") == 0){
    // Save eax and edx
    AsmInstruction* pushEax = create_asm_instruction(NULL, "pushl", NULL, "%eax");
    AsmInstruction* pushEdx = create_asm_instruction(NULL, "pushl", NULL, "%edx");
    AsmInstruction* dividend = create_asm_instruction(NULL, "movl", iloc->operand1, "%eax");
    AsmInstruction* cltd = create_asm_instruction(NULL, "cltd", NULL, NULL);
    AsmInstruction* div = create_asm_instruction(NULL,"idivl" , NULL, iloc->operand2);
    AsmInstruction* movRes = create_asm_instruction(NULL, "movl", "%eax", iloc->operand3);
    AsmInstruction* popEdx = create_asm_instruction(NULL, "popl", NULL, "%edx");
    AsmInstruction* popEax = create_asm_instruction(NULL, "popl", NULL, "%eax");
    pushEax->next = pushEdx; pushEdx->next = dividend; dividend->next = cltd;
    cltd->next = div; div->next = movRes; movRes->next = popEdx; popEdx->next = popEax;
    
    popEax->prev = popEdx; popEdx->prev = movRes; movRes->prev = div; div->prev = cltd;
    cltd->prev = dividend; dividend->prev = pushEdx; pushEdx->prev = pushEax;
    return pushEax;
  } else if (strcmp(iloc->opcode, "i2i") == 0){
    char* x86_reg1 = x86_reg(iloc->operand1);
    char* x86_reg2 = x86_reg(iloc->operand3);
    if(haveAny64BitRegister(x86_reg1, x86_reg2)){
      AsmInstruction* move = create_asm_instruction(NULL, "movq", x86_reg1, x86_reg2);
      return move;
    }
    AsmInstruction* move = create_asm_instruction(NULL, "movl", x86_reg1, x86_reg2);
    return move;
  } else if(strcmp(iloc->opcode, "store") == 0){
    char memory_addr[strlen(iloc->operand1) + 3];
    sprintf(memory_addr, "(%s)", iloc->operand1);

    AsmInstruction* move = create_asm_instruction(NULL, "movl", memory_addr, iloc->operand2);
    return move;
  } else if(strcmp(iloc->opcode, "storeAI") == 0){
    AsmInstruction* move = create_asm_instruction(NULL, "movl", x86_reg(iloc->operand1), x86_offset(iloc->operand2, iloc->operand3));
    return move;
  } else if(strcmp(iloc->opcode, "jump") == 0){
    char memory_addr[strlen(iloc->operand1) + 2];
    sprintf(memory_addr, "*%s", iloc->operand1);
    AsmInstruction* jmp = create_asm_instruction(NULL, "jmp", NULL, memory_addr);
    return jmp;
  } else if(strcmp(iloc->opcode, "jumpI") == 0){
    AsmInstruction* jmp = create_asm_instruction(NULL, "jmp", NULL, iloc->operand1);
    return jmp;
  } else if(strcmp(iloc->opcode, "halt") == 0){
    char* last_temp = prev->dst == NULL? (prev->src == NULL? strdup("$0") : prev->src): prev->dst;
    
    AsmInstruction* return_asm = create_asm_instruction(NULL, "movl", last_temp, "%eax");
    AsmInstruction* popq = create_asm_instruction(NULL, "popq", NULL, "%rbp");
    AsmInstruction* ret = create_asm_instruction(NULL, "ret", NULL, NULL);
    AsmInstruction* final_proc = create_asm_instruction(NULL,"final_proc", ".cfi_endproc", NULL);
    concat_asm_instructions(return_asm, popq);
    concat_asm_instructions(popq, ret);
    concat_asm_instructions(ret, final_proc);
    return return_asm;
  } else if(strcmp(iloc->opcode, "cmp_LT") == 0) {
    return create_asm_cmp_code(iloc, "jl");
  } else if(strcmp(iloc->opcode, "cmp_LE") == 0) {
    return create_asm_cmp_code(iloc, "jle");
  } else if(strcmp(iloc->opcode, "cmp_EQ") == 0) {
    return create_asm_cmp_code(iloc, "je");
  } else if(strcmp(iloc->opcode, "cmp_NE") == 0) {
    return create_asm_cmp_code(iloc, "jne");
  } else if(strcmp(iloc->opcode, "cmp_GT") == 0) {
    return create_asm_cmp_code(iloc, "jg");
  } else if(strcmp(iloc->opcode, "cmp_GE") == 0) {
    return create_asm_cmp_code(iloc, "jge");
  } else if(strcmp(iloc->opcode, "nop") == 0) {
    if (iloc->label != NULL) {
      return create_asm_instruction(iloc->label, NULL, NULL, NULL);
    }
  }

  return NULL;
}

void print_asm_instruction(AsmInstruction* asm_code){
  if(asm_code == NULL) {
    return;
  }

  if(strcmp(asm_code->opcode, "final_proc") == 0){
    printf(".cfi_endproc\n");
  } else if(asm_code->src != NULL && asm_code->dst != NULL){
     printf("\t%s %s,%s\n", asm_code->opcode, asm_code->src, asm_code->dst);
  } else if (asm_code->src == NULL && asm_code->dst != NULL){
    printf("\t%s %s\n", asm_code->opcode, asm_code->dst);
  } else if (asm_code->src != NULL && asm_code->dst == NULL){
    printf("\t%s %s\n", asm_code->opcode, asm_code->src);
  } else if(asm_code->src == NULL && asm_code->dst == NULL){
    printf("\t%s\n", asm_code->opcode);
  } else{
    printf("QUE PORRA QUE DEU AQUI: %s\n", asm_code->opcode);
  }
}

void print_asm_instructions(AsmInstruction* asm_code){
  while(asm_code != NULL){
    print_asm_instruction(asm_code);
    asm_code = asm_code->next;
  }
}

char* x86_literal(char* iloc_literal){
  char* literal = (char*) malloc(strlen(iloc_literal) + 1);
  literal = strdup("$");
  literal = strcat(literal, iloc_literal);

  return literal;
}

char* x86_offset(char* iloc_reg, char* offset){
  // TODO Provavelmente isso aqui vai ter um leak de memória pq na criacao é feito strdup
  char* x86Reg = x86_reg(iloc_reg);
  char* x86Offset = (char*) malloc(sizeof(char) * (strlen(x86_reg(iloc_reg)) + strlen("-()") + strlen(offset) + 1));

  sprintf(x86Offset, "-%s(%s)", offset, x86Reg);
  return x86Offset;
}

char* x86_reg(char* iloc_reg)
{
  if (strcmp("rfp", iloc_reg) == 0)
  {
    return "%rbp";
  }
  else if (strcmp("rsp", iloc_reg) == 0)
  {
    return "%rsp";
  }
  else if (strcmp("rbss", iloc_reg) == 0)
  {
    return "%rsp";
  }
  //TEMPORÁRIO APENAS PARA TESTAR!!!!!!!!!!!!!!!!!
  else if(strcmp("r0", iloc_reg) == 0){
    return "%ecx";
  }
  else if(strcmp("r1", iloc_reg) == 0){
    return "%ebx";
  }
  else if(strcmp("r2", iloc_reg) == 0){
    return "%esi";
  }
  else if(strcmp("r3", iloc_reg) == 0){
    return "%edi";
  }
  else if(strcmp("r4", iloc_reg) == 0){
    return "%r8d";
  }
  else if(strcmp("r5", iloc_reg) == 0){
    return "%r9d";
  }
  else if(strcmp("r6", iloc_reg) == 0){
    return "%r10d";
  }
  else if(strcmp("r7", iloc_reg) == 0){
    return "%r11d";
  }
  else if(strcmp("r8", iloc_reg) == 0){
    return "%r12d";
  }
  else if(strcmp("r9", iloc_reg) == 0){
    return "%r13d";
  }
  else if(strcmp("r10", iloc_reg) == 0){
    return "%r14d";
  }
  else if(strcmp("r11", iloc_reg) == 0){
    return "%r15d";
  }
  else
  {
    return iloc_reg;
  }
};

bool haveAny64BitRegister(char* reg1, char* reg2){
  return is64bitRegister(reg1) || is64bitRegister(reg2);
}

bool is64bitRegister(char* reg){
  if(strcmp(reg, "%rbp") == 0)
    return true;
  if(strcmp(reg, "%rsp") == 0)
    return true;
  return false;
}

void print_init_asm_code(){
  printf("\t.text\n\t.globl	main\n\t.type	main, @function\nmain:\n\t.LFB0:\n\t.cfi_startproc\n");
}

void print_final_asm_code(){
  printf(".LFE0:\n\t.size	main, .-main\n");
}

void print_asm_globals_code(Symbol_Entry** global_scope) {
  bool isFirstGlobal = true;
  for (int i = 0; i < TABLE_SIZE; i++) {
    if (global_scope[i] != NULL) {
      Symbol_Entry* entry = global_scope[i];
      while (entry != NULL) {
        if (entry->nature == VAR) {
          if (isFirstGlobal == true) {
            printf("  .text\n");
            isFirstGlobal = false;
          }
          printf("  .globl %s\n", entry->key);
          printf("  .data\n");
          printf("  .align 4\n"); // depends on machine word length (?)
          printf("  .type %s, @object\n", entry->key);
          printf("  .size %s, %d\n", entry->key, entry->length);
          printf("%s:\n", entry->key);
          printf("  .long 0\n"); // in the future this will depend on type
        }

        entry = entry->next;
      }
    }
  }
}

void update_asm_return_addr(AsmInstruction* head) {
  AsmInstruction* item = head;
  while (item != NULL) {
    bool is_mov_rpc = strcmp(item->opcode, "movl") == 0 && strcmp(item->src, "rpc") == 0;
    bool is_next_add = strcmp(item->next->opcode, "addl") == 0;
    if (is_mov_rpc && is_next_add) {
      AsmInstruction* add_rpc = item->next;
      AsmInstruction* jmp = add_rpc->next;
      int instr_count = 1;
      while (jmp != NULL && strcmp(jmp->opcode, "jmp") != 0 && jmp->dst[0] != '*') {
        jmp = jmp->next;
        instr_count++;
      }
      if (jmp != NULL) {
        char offset_str[18];
        sprintf(offset_str, "$%d", instr_count*5);
        add_rpc->dst = strdup(offset_str);
      }
    }
    item = item->next;
  }
}

AsmInstruction* create_asm_cmp_code(Instruction* iloc_cmp, const char* jmp_type) {
  AsmInstruction* cmp = NULL;
  Instruction* cbr = iloc_cmp->previous;
  if (strcmp("cbr", cbr->opcode) == 0) {
    cmp = create_asm_instruction(NULL, "cmp", iloc_cmp->operand1, iloc_cmp->operand2);
    AsmInstruction* jmp_true = create_asm_instruction(NULL, jmp_type, NULL, cbr->operand2);
    AsmInstruction* jmp_false = create_asm_instruction(NULL, "jmp", NULL, cbr->operand3);
    concat_asm_instructions(cmp, jmp_true);
    concat_asm_instructions(jmp_true, jmp_false);
  }
  return cmp;
}