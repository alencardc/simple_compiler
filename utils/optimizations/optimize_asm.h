#include "../asm/asm.h"

AsmInstruction* optimize_asm_code(AsmInstruction* asm_code);

void optimize_mov(AsmInstruction* current_code);

void remove_instruction(AsmInstruction* asm_code);