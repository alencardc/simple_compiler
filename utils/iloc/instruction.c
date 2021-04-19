#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "instruction.h"

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

Placeholder* concat_placeholders(Placeholder* list1, Placeholder* list2) {
  if (list1 == NULL)
    return list2;
  if (list2 == NULL)
    return list1;
  
  Placeholder* temp = list1;
  while (temp->next != NULL)
    temp = temp->next;
  
  temp->next = list2;
  return list1;
}

Placeholder* create_and_concat_placeholder(char** label, Placeholder* list1, Placeholder* list2) {
  Placeholder* new = create_placeholder(label);
  Placeholder* result = concat_placeholders(list1, list2);
  return concat_placeholders(new, result);
}

void backpatch(Placeholder* list, char* label) {
  if (list == NULL || label == NULL)
    return;
    
  Placeholder* temp = list;
  while (temp != NULL) {
    *(temp->label_ref) = strdup(label);
    temp = temp->next;
  }
}

Instruction* create_instruction(
  const char* opcode, 
  const char* arg1, 
  const char* arg2, 
  const char* arg3,
  Instruction* prev
) {
  Instruction* instruction = (Instruction*) malloc(sizeof(Instruction));

  if (instruction == NULL) {
    return NULL;
  }

  instruction->label = NULL;
  instruction->opcode = strdup(opcode);
  instruction->operand1 = arg1 != NULL ? strdup(arg1) : NULL;
  instruction->operand2 = arg2 != NULL ? strdup(arg2) : NULL;
  instruction->operand3 = arg3 != NULL ? strdup(arg3) : NULL;
  instruction->previous = prev;
  
  return instruction;
}

Instruction* create_label(const char* label, Instruction* prev) {
  if (label == NULL)
    return NULL;
  
  Instruction* instruction = (Instruction*) malloc(sizeof(Instruction));
  if (instruction == NULL) {
    return NULL;
  }

  instruction->label = strdup(label);
  instruction->opcode = strdup("nop");
  instruction->operand1 = NULL;
  instruction->operand2 = NULL;
  instruction->operand3 = NULL;
  instruction->previous = prev;
  
  return instruction;
}

void free_instruction(Instruction* instr) {
  if (instr == NULL)
    return;
  
  //print_instruction(instr);
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

  if (i->label != NULL) {
    printf("%s: %s\n", i->label, i->opcode != NULL ? i->opcode : "");
  }
  else if (strcmp("loadAI", i->opcode) == 0) {
    printf("loadAI %s, %s => %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("storeAI", i->opcode) == 0) {
    printf("storeAI %s => %s, %s\n", i->operand1, i->operand2, i->operand3);
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
  else if (strcmp("cbr", i->opcode) == 0) {
    printf("cbr %s -> %s, %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("jumpI", i->opcode) == 0) {
    printf("jumpI %s\n", i->operand1);
  }
  else if (strcmp("cmp_LT", i->opcode) == 0) {
    printf("cmp_LT %s, %s -> %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("cmp_LE", i->opcode) == 0) {
    printf("cmp_LE %s, %s -> %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("cmp_EQ", i->opcode) == 0) {
    printf("cmp_EQ %s, %s -> %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("cmp_NE", i->opcode) == 0) {
    printf("cmp_NE %s, %s -> %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("cmp_GT", i->opcode) == 0) {
    printf("cmp_GT %s, %s -> %s\n", i->operand1, i->operand2, i->operand3);
  }
  else if (strcmp("cmp_GE", i->opcode) == 0) {
    printf("cmp_GE %s, %s -> %s\n", i->operand1, i->operand2, i->operand3);
  }
}

void print_iloc_code(Instruction* head) {
  
  Instruction* instr = reverse_instr_list(head);
  while (instr != NULL) {
    print_instruction(instr);
    instr = instr->previous;
  }
}

Instruction* reverse_instr_list(Instruction* list) {
  Instruction* next = NULL;
  Instruction* curr = list;
  Instruction* prev = NULL;

  while (curr != NULL) {
    next = curr->previous;
    curr->previous = prev;
    prev = curr;
    curr = next;
  }
  list = prev;
  return list;
}