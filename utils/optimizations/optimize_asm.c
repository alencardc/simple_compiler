#include "./optimize_asm.h"

AsmInstruction* optimize_asm_code(AsmInstruction* asm_code){
    AsmInstruction* current_code = asm_code;
    while(current_code != NULL){
        //Search for mov src -> src
        optimize_redundant_mov(current_code);
        optimize_literal_constants_reg(current_code);
        optimize_inc_dec(current_code);
        optimize_jmp_to_next_instruction(current_code);
        optimize_mult_div_stack_op(current_code);
optimize_intermediary_registers(current_code);
        current_code = current_code->next;
    }

    return asm_code;
}

void optimize_mult_div_stack_op(AsmInstruction* current_code){
    if(current_code->opcode == NULL){
        return;
    }

    if(strcmp(current_code->opcode, "imull") == 0
        || strcmp(current_code->opcode, "idivl") == 0){
        AsmInstruction* mult_or_div = current_code;
        //Remove pushq before imull/idivl
        remove_instruction(current_code->prev->prev); //First push makes second push only
        remove_instruction(current_code->prev->prev); //2 instruction ahead 
        //Remove popq after imull/idivl
        remove_instruction(current_code->next->next); //First pop makes second pop only
        remove_instruction(current_code->next->next); //2 instruction ahead        
    }
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

    if(strcmp(current_code->dst, "%eax") == 0)
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

void optimize_jmp_to_next_instruction(AsmInstruction* asm_code){
    if(asm_code == NULL || asm_code->opcode == NULL 
        || asm_code->dst == NULL)
        return;

    if(strcmp(asm_code->opcode, "jmp") == 0){
        AsmInstruction* next_instruction = asm_code->next;
        if(strcmp(next_instruction->label, asm_code->dst) == 0){
            remove_instruction(asm_code);
        }
    }
}

void optimize_intermediary_registers(AsmInstruction* asm_code){
    if(asm_code == NULL || asm_code->opcode == NULL 
        || asm_code->src == NULL || asm_code->dst == NULL)
        return;
    
    if(strcmp(asm_code->opcode, "movl") == 0){
        char* dst_register = asm_code->dst;
        char* new_src = asm_code->src;

        //Exclude access to memory and final eax from otimizations, we just optimize regs
        if(strstr(dst_register, "rbp") != NULL ||
        strstr(dst_register, "rip") != NULL ||
        strstr(dst_register, "rsp") != NULL ||
        strstr(dst_register, "eax") != NULL)
            return;

        AsmInstruction* temp = asm_code->next;
        while(temp != NULL){
                //If reg is used as a dst after that, it wasnt a intermediaryt reg
                if(temp->dst != NULL && strcmp(temp->dst, dst_register) == 0){
                    return;
                }
                temp = temp->next;
        }
        temp = asm_code->next;

        while(temp != NULL){
            if(temp->src != NULL && strcmp(temp->src, dst_register) == 0){
                    free(temp->src);
                    temp->src = strdup(new_src);
                }
                temp = temp->next;
        }
        remove_instruction(asm_code);
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
