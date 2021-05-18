#include "./optimize_asm.h"

AsmInstruction* optimize_asm_code(AsmInstruction* asm_code){
    AsmInstruction* current_code = asm_code;
    while(current_code != NULL){
        //Search for mov src -> src
        optimize_redundant_mov(current_code);
        optimize_literal_constants_reg(current_code);
        optimize_inc_dec(current_code);
        current_code = current_code->next;
    }

    return asm_code;
}

void optimize_redundant_mov(AsmInstruction* current_code){
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

void optimize_literal_constants_reg(AsmInstruction* current_code){
    if(current_code->opcode == NULL || current_code->src == NULL || current_code->dst == NULL)
        return;

    if(strcmp(current_code->opcode, "movl") == 0){
        if(current_code->src[0] == '$'){
            char* const_literal = current_code->src;
            char* prov_const_reg = strdup(current_code->dst);
            AsmInstruction* temp = current_code->next;
            while(temp != NULL){
                //If reg is used as a dst after that, it wasnt a const reg
                if(temp->dst != NULL && strcmp(temp->dst, prov_const_reg) == 0){
                    free(prov_const_reg);
                    return;
                }
                temp = temp->next;
            }

            remove_instruction(current_code);
            temp = current_code->next;
            while(temp != NULL){
                if(temp->src != NULL && strcmp(temp->src, prov_const_reg) == 0){
                    free(temp->src);
                    temp->src = const_literal;
                }
                temp = temp->next;
            }
        }
    }

}

void optimize_inc_dec(AsmInstruction* asm_code){
    if(asm_code == NULL || asm_code->opcode == NULL 
        || asm_code->src == NULL || asm_code->dst == NULL)
        return;
    
    if(strcmp(asm_code->src, "$1") == 0){
        if(strcmp(asm_code->opcode, "subl") == 0){
            free(asm_code->opcode);
            free(asm_code->src);
            asm_code->opcode = strdup("dec");
            asm_code->src = NULL;
        }
        else if(strcmp(asm_code->opcode, "addl") == 0){
            free(asm_code->opcode);
            free(asm_code->src);
            asm_code->opcode = strdup("inc");
            asm_code->src = NULL;
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
