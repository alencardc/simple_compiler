#ifndef __PROD_NODE_H__
#define __PROD_NODE_H__

#include "ast/ast.h"
#include "iloc/instruction.h"

typedef struct sProdNode {
  Node* ast;
  Instruction* instr;
} ProdNode;

#endif // __PROD_NODE_H__