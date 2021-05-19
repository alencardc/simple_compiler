#ifndef __OPTIMIZE_ASM_H__
#define __OPTIMIZE_ASM_H__

#include "../asm/asm.h"

AsmInstruction* optimize_asm_code(AsmInstruction* asm_code);

void optimize_redundant_mov(AsmInstruction* current_code);

void optimize_literal_constants_reg(AsmInstruction* current_code);
void optimize_inc_dec(AsmInstruction* asm_code);
void optimize_jmp_to_next_instruction(AsmInstruction* asm_code);
void optimize_mult_div_stack_op(AsmInstruction* current_code);
void optimize_intermediary_registers(AsmInstruction* asm_code);

void remove_instruction(AsmInstruction* asm_code);

#endif // __OPTIMIZE_ASM_H__