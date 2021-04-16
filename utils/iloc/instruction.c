#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "instruction.h"

Instruction* create_instruction(
  const char* opcode, 
  const char* arg1, 
  const char* arg2, 
  const char* arg3,
  Instruction* prev
) {
  Instruction* instruction = (Instruction*) malloc(sizeof(Instruction));

  if (instruction == NULL || arg1 == NULL) {
    return NULL;
  }

  instruction->opcode = strdup(opcode);
  instruction->operand1 = arg1 != NULL ? strdup(arg1) : NULL;
  instruction->operand2 = arg2 != NULL ? strdup(arg2) : NULL;
  instruction->operand3 = arg3 != NULL ? strdup(arg3) : NULL;
  instruction->previous = prev;
  
  return instruction;
}

void free_instruction(Instruction* instr) {
  if (instr == NULL)
    return;
  
  print_instruction(instr);
  while (instr->previous != NULL) {
    free_instruction(instr->previous);}

  free(instr->opcode);
  free(instr->operand1);
  free(instr->operand2);
  free(instr->operand3);
  free(instr);
  instr = NULL;
  return;
}

Instruction* insert_instr_begin(Instruction* head, Instruction* new_instr) {
  if (head == NULL)
    return new_instr;
  if (new_instr == NULL)
    return head;
  
  new_instr->previous = head;
  return new_instr;
}

Instruction* concat_instructions(Instruction* head, Instruction* new_instr) {
  if (head == NULL) 
    return new_instr;
  if (new_instr == NULL) 
    return head;
  
  Instruction* temp = head;
  while (temp->previous != NULL) 
    temp = temp->previous;
  
  temp->previous = new_instr;
  return head;
}



void print_instruction(Instruction* i) {
  if (i == NULL)
    return;

  if (strcmp("loadAI", i->opcode) == 0) {
    printf("loadAI %s, %s => %s\n", i->operand1, i->operand2, i->operand3);
  } 
  else if (strcmp("loadI", i->opcode) == 0) {
    printf("loadI %s => %s\n", i->operand1, i->operand3);
  }
  else if (strcmp("rsubI", i->opcode) == 0) {
    printf("rsubI %s, %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("add", i->opcode) == 0) {
    printf("add %s, %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("sub", i->opcode) == 0) {
    printf("sub %s, %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("mult", i->opcode) == 0) {
    printf("mult %s, %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("div", i->opcode) == 0) {
    printf("div %s, %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
}

void print_iloc_code(Instruction* head) {
  
  Instruction* instr = head;
  while (instr != NULL) {
    print_instruction(instr);
    instr = instr->previous;
  }
}

Placeholder* create_placeholder(char** label) {
  Placeholder* placeholder = (Placeholder*) malloc(sizeof(Placeholder));
  placeholder->label_ref = label;
  return placeholder;
}

void free_placeholder(Placeholder* placeholder) {
  Placeholder* temp;
  while (placeholder != NULL) {
    temp = placeholder;
    placeholder = placeholder->next;
    free(temp);
    temp = NULL;
  }
}

Placeholder* insert_placeholder(Placeholder* new_item, Placeholder* list) {
  if (list == NULL)
    return new_item;
  if (new_item == NULL)
    return list;
  
  new_item->next = list;
  return new_item;
}