#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./optimize_iloc.h"
#include "hashtable.h"

Instruction* optimize_iloc_code(Instruction* iloc_code) {
    printf("Optimizing iloc code....\n");
    //Super optimized iloc_code
    optimize_iloc_expressions(iloc_code);
    iloc_code = iloc_remove_deleted(iloc_code);

    return iloc_code;
}

void optimize_iloc_expressions(Instruction* iloc_code) {
    HashTable* ht = ht_init();
    Instruction* instr = iloc_code;
    while (instr != NULL) {
        if (strcmp(instr->opcode, "loadI") == 0) {
            HashEntry* entry = ht_create_entry(instr->operand3, instr);
            ht_insert(entry, ht);
        
        } else if (strcmp(instr->opcode, "add") == 0
            || strcmp(instr->opcode, "sub") == 0
            || strcmp(instr->opcode, "mul") == 0
            || strcmp(instr->opcode, "div") == 0
        ) {
            HashEntry* r1 = ht_get_entry(instr->operand1, ht);
            HashEntry* r2 = ht_get_entry(instr->operand2, ht);
            if (r1 != NULL && r2 != NULL) {
                // Check if r1 or r2 is used after this instr
                bool will_be_used = false;
                Instruction* searcher = instr->previous;
                while (searcher != NULL && will_be_used == false) {
                    if (searcher->operand1 != NULL
                        && (strcmp(searcher->operand1, r1->key) == 0
                        || strcmp(searcher->operand1, r2->key) == 0)
                    ) {
                        will_be_used = true;
                    } else if (searcher->operand2 != NULL
                        && (strcmp(searcher->operand2, r1->key) == 0
                        || strcmp(searcher->operand2, r2->key) == 0)
                    ) {
                        will_be_used = true;
                    }
                    searcher = searcher->previous;
                }

                if (will_be_used == false) {
                    // Mark as "deleted" the loadI and add   instructions
                    Instruction* instr1 = r1->value;
                    Instruction* instr2 = r2->value;
                    instr->comment = strdup("//deleted");
                    instr1->comment = strdup("//deleted");
                    instr2->comment = strdup("//deleted");
                    // Add new loadI after add instruction
                    int value = atoi(instr1->operand1) + atoi(instr2->operand1);
                    char str_val[16];
                    sprintf(str_val, "%d", value);
                    Instruction* load = create_instruction("loadI", str_val, NULL, instr->operand3, NULL);
                    load->previous = instr->previous;
                    instr->previous = load;
                }
            }
        }

        instr = instr->previous;
    }
}

Instruction* iloc_remove_deleted(Instruction* iloc_code) {
    Instruction* prev = NULL;
    Instruction* curr = iloc_code;

    while (curr != NULL) {
        if (curr->comment != NULL && strcmp(curr->comment, "//deleted") == 0) {
            Instruction* next = curr->previous;

            if (curr == iloc_code)
                iloc_code = next;

            curr->previous = NULL;
            if (prev != NULL)
                prev->previous = next;
            
            free_instruction(curr);
            curr = next;
        } else {
            prev = curr;
            curr = curr->previous;
        }
    }
    return iloc_code;
}