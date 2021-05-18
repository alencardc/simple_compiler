#ifndef __OPTIMIZE_ILOC_H__
#define __OPTIMIZE_ILOC_H__

#include "../iloc/instruction.h"

Instruction* optimize_iloc_code(Instruction* iloc_code);

void optimize_iloc_expressions(Instruction* iloc_code);
Instruction* iloc_remove_deleted(Instruction* iloc_code);

#endif //  __OPTIMIZE_ILOC_H__