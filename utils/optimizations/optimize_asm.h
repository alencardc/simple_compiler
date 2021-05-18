#include "../asm/asm.h"

AsmInstruction* optimize_asm_code(AsmInstruction* asm_code);

void optimize_redundant_mov(AsmInstruction* current_code);

void optimize_literal_constants_reg(AsmInstruction* current_code);
void optimize_inc_dec(AsmInstruction* asm_code);

void remove_instruction(AsmInstruction* asm_code);