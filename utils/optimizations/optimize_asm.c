#include "./optimize_asm.h"

AsmInstruction* optimize_asm_code(AsmInstruction* asm_code){
    AsmInstruction* current_code = asm_code;
    while(current_code != NULL){
        //Search for mov src -> src
        optimize_mov(current_code);
        current_code = current_code->next;
    }

    return asm_code;
}

void optimize_mov(AsmInstruction* current_code){
    if(current_code->opcode == NULL){
        return;
    }

    if(current_code->src == NULL || current_code->dst == NULL)
        return;

    if(strcmp(current_code->opcode,"movq") == 0 || strcmp(current_code->opcode,"movl") == 0){
        if(strcmp(current_code->src, current_code->dst) == 0){
            remove_instruction(current_code);
        }
    }
}

void remove_instruction(AsmInstruction* asm_code){
    AsmInstruction* previous = asm_code->prev;
    AsmInstruction* next = asm_code->next;

    if(next != NULL){
        next->prev = previous;
    }
    if(previous != NULL){
        previous->next = next;
    }
}
