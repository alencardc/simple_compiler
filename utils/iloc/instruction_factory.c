#include "instruction_factory.h"

static char* get_new_register() {
  static int register_id = 0;
  int label_size = 1 + 1; // Letter r or L + \0
  label_size += 1 + register_id/10; // Digits required to represent register_id

  char* label = (char*) malloc(label_size*sizeof(char));
  sprintf(label, "r%d", register_id);

  register_id += 1;
  return label;
}

static char* get_new_label() {
  static int label_id = 0;
  int label_size = 1 + 1; // Letter r or L + \0
  label_size += 1 + label_id/10; // Digits required to represent label_id

  char* label = (char*) malloc(label_size*sizeof(char));
  sprintf(label, "L%d", label_id);

  label_id += 1;
  return label;
}

void create_instr_identifier(Node* node, Table_Stack* scopes) {
  if (node == NULL || scopes == NULL)
    return;
  
  Symbol_Entry* entry = search_all_scopes(scopes, node->label);
  if (entry == NULL) {
    node->temp = NULL;
    node->instr = NULL;
    return;
  }

  char* r = get_new_register();
  node->temp = r;

  char offset[12];
  sprintf(offset, "%d", entry->offset);

  if (entry->global == true) {
    node->instr = create_instruction("loadAI", "rbss", offset, r, NULL);
  } else {
    node->instr = create_instruction("loadAI", "rfp", offset, r, NULL);
  }
}

void create_instr_literal(Node* node, Table_Stack* scopes) {
  if (node == NULL || scopes == NULL)
    return;

  Symbol_Entry* entry = search_all_scopes(scopes, node->label);
  if (entry == NULL) {
    node->temp = NULL;
    node->instr = NULL;
    return;
  }
  
  char* r = get_new_register();
  node->temp = r;
  
  switch (entry->type) {
    case INTEGER_VAL:
      node->instr = create_instruction("loadI", entry->key, NULL, r, NULL);
      break;
    case BOOL_VAL:
      node->instr = create_instruction("loadI", entry->key, NULL, r, NULL);
      Instruction* jump = create_instruction("jumpI", NULL, NULL, NULL, node->instr);
      node->instr = jump;
      if (node->data->token_value.b_val == true) {
        node->tl = create_placeholder(&jump->operand1);
        node->fl = NULL;
      } else {
        node->tl = NULL;
        node->fl = create_placeholder(&jump->operand1);
      }
  }
}

void create_instr_from_local_list(Node* list, Table_Stack* scopes) {
  if (list == NULL || scopes == NULL)
    return;

  Instruction* instr_list = NULL;
  Node* node = list;
  while (node != NULL) {
    Symbol_Entry* entry = NULL;
    if (node->type == AST_ASSIGN)
      entry = get_entry_from_table(node->children[0]->label, scopes->table);
    else
      entry = get_entry_from_table(node->label, scopes->table);
    
    char buff[12];
    sprintf(buff, "%d", entry->length);
    Instruction* instr = create_instruction("addI", "rsp", buff, "rsp", NULL);

    if (node->type == AST_ASSIGN) {
      Node* value = node->children[1];
      if (value->type == AST_LITERAL) {
        create_instr_literal(value, scopes);
      } else {
        create_instr_identifier(value, scopes);
      }
      sprintf(buff, "%d", entry->offset);
      instr = concat_instructions(value->instr, instr);
      instr = create_instruction("storeAI", value->instr->operand3, "rfp", buff, instr);

      node->tl = value->tl;
      node->fl = value->fl;  
    }
    node->instr = instr;
    instr_list = concat_instructions(node->instr, instr_list);

    if (node->children_amount > expected_children_amount(node->type)) {
      node = node->children[node->children_amount-1];
    } else {
      node = NULL;
    }
  }
  list->instr = instr_list;
}

void create_instr_init(Node* init, Node* id, Node* value) {}

void create_instr_unary(Node* op_node, Node* operand) {
  if (op_node == NULL || operand->temp == NULL) {
    return;
  }

  Instruction* instr;
  if (op_node->label[0] == '-') {
    char* r = get_new_register();
    op_node->temp = r;
    instr = create_instruction("rsubI", operand->temp, "0", op_node->temp, NULL);
    //free(operand->temp); operand->temp = NULL;
    op_node->instr = insert_instr_begin(operand->instr, instr);
  }
  else if (op_node->label[0] == '!') {
    op_node->tl = operand->fl;
    op_node->fl = operand->tl;
    op_node->instr = operand->instr;
    // What about op_node->temp?
  }
  else {
    // Not implement yet, just concat instructions and lists
    op_node->tl = operand->tl;
    op_node->fl = operand->fl;
    op_node->instr = operand->instr;
  }

}

void create_instr_binary(Node* op_node, Node* left, Node* right) {
  
  create_instr_arith_op(op_node, left, right);
  create_instr_rel_op(op_node, left, right);
  create_instr_log_op(op_node, left, right);

  // Not sure if it is needed yet int the create_instr_... above
  // if (instr == NULL) {
  //   free(op_node->temp);
  //   op_node->temp = NULL;
  //   op_node->temp = left->temp;
  //   free(right->temp);
  //   right->temp = NULL;
  // } else {
  //   free(left->temp); free(right->temp); 
  //   left->temp = NULL; right->temp = NULL;
  // }
}

bool create_instr_arith_op(Node* op_node, Node* left, Node* right) {
  if (op_node == NULL || left->temp == NULL || right->temp == NULL)
    return false;

  char* opcode = get_arithop_instr(op_node->label);
  if (opcode == NULL)
    return false;

  char* r = get_new_register();
  op_node->temp = r;
  Instruction* instr = create_instruction(opcode, left->temp, right->temp, op_node->temp, NULL);
  Instruction* list = concat_instructions(instr, right->instr);
  op_node->instr = concat_instructions(list, left->instr);

  free(opcode);
  return true;
}

bool create_instr_rel_op(Node* op_node, Node* left, Node* right) {
  if (op_node == NULL || left->temp == NULL || right->temp == NULL)
    return false;

  char* opcode = get_relop_instr(op_node->label);
  if (opcode == NULL)
    return false;

  char* r = get_new_register();
  op_node->temp = r;
  Instruction* instr = create_instruction(opcode, left->temp, right->temp, op_node->temp, NULL);
  instr = create_instruction("cbr", op_node->temp, NULL, NULL, instr);
  op_node->tl = create_and_concat_placeholder(&instr->operand2, left->tl, right->tl);
  op_node->fl = create_and_concat_placeholder(&instr->operand3, left->fl, right->fl);
  
  Instruction* list = concat_instructions(instr, right->instr);
  op_node->instr = concat_instructions(list, left->instr);

  free(opcode);
  return true;
}

bool create_instr_log_op(Node* op_node, Node* left, Node* right) {
  if (op_node == NULL || left == NULL || right == NULL)
    return false;

  bool is_log_op = false;
  char* new_label = NULL;
  if (strcmp(op_node->label, "&&") == 0) {
    new_label = get_new_label();
    backpatch(left->tl, new_label);
    op_node->tl = right->tl;
    op_node->fl = concat_placeholders(left->fl, right->fl);
    is_log_op = true;
  }
  else if (strcmp(op_node->label, "||") == 0) {
    new_label = get_new_label();
    backpatch(left->fl, new_label);
    op_node->fl = right->fl;
    op_node->tl = concat_placeholders(left->tl, right->tl);
    is_log_op = true;
  }

  if (is_log_op == true) {
    Instruction* label_instr = create_label(new_label, left->instr);
    op_node->instr = concat_instructions(right->instr, label_instr);
  }

  free(new_label);
  return is_log_op;
}

void evaluate_expression(Node* exp) {
  if (exp == NULL)
    return;

  if (exp->tl == NULL && exp->fl == NULL)
    return;

  char* result_reg = get_new_register();
  char* end_label = get_new_label();
  char* true_label = get_new_label();
  char* false_label = get_new_label();
  backpatch(exp->tl, true_label);
  backpatch(exp->fl, false_label);

  Instruction* true_label_instr = create_label(true_label, exp->instr);
  Instruction* load_true = create_instruction("loadI", "1", NULL, result_reg, true_label_instr);
  Instruction* jump = create_instruction("jumpI", end_label, NULL, NULL, load_true);
  Instruction* false_label_instr = create_label(false_label, jump);
  Instruction* load_false = create_instruction("loadI", "0", NULL, result_reg, false_label_instr);
  jump = create_instruction("jumpI", end_label, NULL, NULL, load_false);
  Instruction* end_label_instr = create_label(end_label, jump);

  exp->temp = result_reg;
  exp->instr = end_label_instr;

  free(end_label);
  free(true_label);
  free(false_label);
}

char* get_arithop_instr(const char* op) {
  if (op == NULL)
    return NULL;
  
  if (strcmp(op, "+") == 0) {
    return strdup("add");
  } 
  else if (strcmp(op, "-") == 0) {
    return strdup("sub");
  } 
  else if (strcmp(op, "*") == 0) {
    return strdup("mult");
  } 
  else if (strcmp(op, "/") == 0) {
    return strdup("div");
  }

  return NULL;
}

char* get_relop_instr(const char* op) {
  if (op == NULL)
    return NULL;

  if (strcmp(op, ">") == 0) {
    return strdup("cmp_GT");
  } 
  else if (strcmp(op, "<") == 0) {
    return strdup("cmp_LT");
  } 
  else if (strcmp(op, ">=") == 0) {
    return strdup("cmp_GE");
  } 
  else if (strcmp(op, "<=") == 0) {
    return strdup("cmp_LE");
  }
  else if (strcmp(op, "==") == 0) {
    return strdup("cmp_EQ");
  }
  else if (strcmp(op, "!=") == 0) {
    return strdup("cmp_NE");
  }

  return NULL;
}

void create_instr_if(Node* if_node, Node* exp, Node* block) {
  if (exp == NULL)
    return;
  
  Instruction* code_block = NULL;
  if (block != NULL)
    code_block = block->instr;

  char* true_label = get_new_label();
  char* false_label = get_new_label();
  backpatch(exp->tl, true_label);
  backpatch(exp->fl, false_label);
  
  Instruction* true_label_instr = create_label(true_label, NULL);
  Instruction* false_label_instr = create_label(false_label, NULL);
  //if_node->instr = concat_instructions(false_label_instr, true_label_instr);
  if_node->instr = concat_instructions(true_label_instr, exp->instr);
  if_node->instr = concat_instructions(code_block, if_node->instr);
  if_node->instr = concat_instructions(false_label_instr, if_node->instr);

  free(true_label);
  free(false_label);
}

void create_instr_if_else(Node* if_else_node, Node* if_node, Node* block) {
  if (if_else_node == NULL && if_node == NULL)
    return;

  Instruction* code_block = NULL;
  if (block != NULL)
    code_block = block->instr;
  
  // Find false label
  Instruction* false_label_instr = if_node->instr;
  if (false_label_instr->label == NULL)
    return;
  
  // Find true label
  Instruction* true_label_instr = if_node->instr->previous;
  while (true_label_instr != NULL && true_label_instr->label == NULL)
    true_label_instr = true_label_instr->previous;
  
  if (true_label_instr == NULL || true_label_instr->label == NULL)
    return;

  // Label to end of if_else
  char* end_label = get_new_label();

  // Insert true jump before false label to jump the else code
  Instruction* true_jump = create_instruction("jumpI", end_label, NULL, NULL, NULL);
  true_jump->previous = false_label_instr->previous;
  false_label_instr->previous = true_jump;

  // Concatenate code from else block
  if_else_node->instr = concat_instructions(code_block, false_label_instr);
  // Create else jump to end of if else and concatenate with end label instruction
  if_else_node->instr = create_instruction("jumpI", end_label, NULL, NULL, if_else_node->instr);
  if_else_node->instr = create_label(end_label, if_else_node->instr);

  free(end_label);
}

void create_instr_while(Node* while_node, Node* exp, Node* block) {
  if (while_node == NULL && exp == NULL)
    return;
  
  Instruction* code_block = NULL;
  if (block != NULL)
    code_block = block->instr;

  char* true_label = get_new_label();
  char* false_label = get_new_label();
  char* loop_label = get_new_label();

  backpatch(exp->tl, true_label);
  backpatch(exp->fl, false_label);

  Instruction* temp = NULL;
  Instruction* loop_label_instr = create_label(loop_label, NULL);
  temp = concat_instructions(exp->instr, loop_label_instr);
  Instruction* true_label_instr = create_label(true_label, temp);
  temp = concat_instructions(code_block, true_label_instr);
  Instruction* jump = create_instruction("jumpI", loop_label, NULL, NULL, temp);
  Instruction* false_label_instr = create_label(false_label, jump);

  while_node->instr = false_label_instr;
  free(true_label);
  free(false_label);
  free(loop_label);
}

void create_instr_for(Node* for_node, Node* assign, Node* exp, Node* inc, Node* block) {
  if (for_node == NULL && assign == NULL && inc == NULL && exp == NULL)
    return;

  Instruction* code_block = NULL;
  if (block != NULL)
    code_block = block->instr;

  char* true_label = get_new_label();
  char* false_label = get_new_label();
  char* loop_label = get_new_label();

  backpatch(exp->tl, true_label);
  backpatch(exp->fl, false_label);

  Instruction* temp = NULL;
  Instruction* loop_label_instr = create_label(loop_label, assign->instr);
  temp = concat_instructions(exp->instr, loop_label_instr);
  Instruction* true_label_instr = create_label(true_label, temp);
  temp = concat_instructions(code_block, true_label_instr);
  temp = concat_instructions(inc->instr, temp);
  Instruction* jump = create_instruction("jumpI", loop_label, NULL, NULL, temp);
  Instruction* false_label_instr = create_label(false_label, jump);

  for_node->instr = false_label_instr;
  free(true_label);
  free(false_label);
  free(loop_label);
}

void create_instr_assignment(Node* head, Node* id, Table_Stack* scopes, Node* exp){
  if(head == NULL || id == NULL || scopes == NULL || exp == NULL){
    return;
  }

  Symbol_Entry* var_entry = search_all_scopes(scopes, id->label);
  char* base_register;
  if(var_entry->global){
    base_register = strdup("rbss");
  } else{
    base_register = strdup("rfp");
  }
  
  char offset[12];
  sprintf(offset, "%d", var_entry->offset);
  
  //storeAI temp => rbss|rfp,offset
  Instruction* assignment_instr = create_instruction("storeAI", exp->temp, base_register, offset, exp->instr);
  head->instr = assignment_instr;
  free(base_register);
}

extern char* function_id;
void create_instr_return(Node* return_node, Node* exp, Table_Stack* scopes) {

  Symbol_Entry* entry = get_entry_from_table(exp->label, scopes->table);
  Symbol_Entry* curr_function = search_deep_scope(scopes, function_id);

  if (curr_function == NULL || curr_function->nature != FUNCTION)
    return;
  
  if (entry == NULL) {
    // Returned value is a expression
    evaluate_expression(exp);
  }

  if (strcmp(function_id, "main") == 0){
    return_node->instr = create_instruction("halt", NULL, NULL, NULL, exp->instr);
    return;
  }

  int return_offset = curr_function->return_offset;
  char str_offset[12];
  sprintf(str_offset, "%d", return_offset);

  char* return_reg = exp->temp;
  Instruction* store = create_instruction("storeAI", return_reg, "rfp", str_offset, exp->instr);

  char* ret_addr_reg = /*store->operand1 == NULL ?*/ get_new_register() /*: store->operand1*/; // reuse
  Instruction* ret_addr = create_instruction("loadAI", "rfp", "0", ret_addr_reg, store);
  char* rsp_reg = get_new_register();
  Instruction* rsp_load = create_instruction("loadAI", "rfp", "4", rsp_reg, ret_addr);
  char* rfp_reg = get_new_register();
  Instruction* rfp_load = create_instruction("loadAI", "rfp", "8", rfp_reg, rsp_load);
  Instruction* rsp_store = create_instruction("i2i", rsp_reg, NULL,  "rsp", rfp_load);
  Instruction* rfp_store = create_instruction("i2i", rfp_reg, NULL, "rfp", rsp_store);
  Instruction* jump = create_instruction("jump", ret_addr_reg, NULL, NULL, rfp_store);

  return_node->instr = jump;
  free(ret_addr_reg);
  free(rsp_reg);
  free(rfp_reg);
}

Instruction* create_start_function_code(char* function_id, Table_Stack* scopes){
  char* function_label = get_new_label();
  Instruction* label_start = create_label(function_label, NULL);

  Symbol_Entry* function_entry = search_deep_scope(scopes, function_id);
  function_entry->function_label = strdup(function_label);
  free(function_label);

  if (strcmp(function_id, "main") == 0){
    Instruction* init_rsp = create_instruction("i2i", "rfp", NULL, "rsp", label_start);
    return init_rsp;
  }

  int rsp_offset = scopes->offset;
  char rsp_offset_str[12];
  sprintf(rsp_offset_str, "%d", rsp_offset);
  
  Instruction* att_rfp = create_instruction("i2i", "rsp", NULL, "rfp", label_start);
  Instruction* att_rsp = create_instruction("addI", "rsp", rsp_offset_str,  "rsp", att_rfp);

  return att_rsp;  
}

Instruction* create_function_call_code(char* function_id, Table_Stack* scopes, Node* arguments, Node* function_call){
  char* function_label = search_deep_scope(scopes, function_id)->function_label;
  
  //Instructions to save rsp and rfp on function frame
  Instruction* save_rsp = create_instruction("storeAI", "rsp", "rsp", "4", NULL);
  Instruction* save_rfp = create_instruction("storeAI", "rfp", "rsp", "8", save_rsp);
  
  //Generate instructions to save params on function frame
  Instruction* save_params = create_params_save(arguments, scopes);
  save_params = concat_instructions(save_params, save_rfp);

  Instruction* jump_to_function = create_instruction("jumpI", function_label, NULL, NULL, save_params);
 
  //Generate instructions to  save return address
  int num_instructions = count_instructions(jump_to_function);
  char offset_rpc[12];
  sprintf(offset_rpc, "%d", num_instructions + 2);
  char* new_register = get_new_register();
  
  Instruction* store_hole = create_instruction("$store$", NULL, NULL, NULL, NULL);
  Instruction* calc_new_rpc = create_instruction("addI", "rpc", offset_rpc, new_register, store_hole);
  Instruction* save_return_addr = create_instruction("storeAI", new_register, "rsp", "0", calc_new_rpc);
  //Concatenate the save return address instructions before save_rsp and save_rfp
  save_rsp->previous = save_return_addr;

  //loadAI rsp, 16 => r0 
  int return_offset = search_deep_scope(scopes, function_id)->return_offset;
  char return_offset_str[12];
  sprintf(return_offset_str, "%d", return_offset);
  char* return_register = get_new_register();
  Instruction* put_return_on_temp = create_instruction("loadAI", "rsp", return_offset_str, return_register, jump_to_function);
  function_call->temp = return_register;

  Instruction* load_hole = create_instruction("$load$", NULL, NULL, NULL, put_return_on_temp);
  

  free(new_register);
  return load_hole;
}

Instruction* create_params_save(Node* arguments, Table_Stack* scopes){

  int offset = 12;
  Instruction* previous_instructions = NULL;
  Node* current_argument = arguments;
  while (current_argument != NULL)
  {
    char* temp = current_argument->temp;
    char rsp_offset[12];
    sprintf(rsp_offset, "%d", offset);
    Instruction* store_param = create_instruction("storeAI", temp, "rsp", rsp_offset, current_argument->instr);
    
    previous_instructions = concat_instructions(store_param, previous_instructions);
    offset += get_type_lenght(current_argument->value_type);
    if(current_argument->children_amount > expected_children_amount(current_argument->type)){
      current_argument = current_argument->children[current_argument->children_amount - 1];
    } else{
      current_argument = NULL;
    }
  }
  
  return previous_instructions;
}

void create_program_start_instr(Node* node, Table_Stack* scopes) {
  if (node == NULL)
    return;
  
  Symbol_Entry* main_func = search_deep_scope(scopes, "main");
  if (main_func->nature == FUNCTION) {
    Instruction* rfp_load =  create_instruction("loadI", "1024", NULL, "rfp", NULL);
    Instruction* rsp_load =  create_instruction("loadI", "1024", NULL, "rsp", rfp_load);
    int instruction_count = count_instructions(node->instr);
    instruction_count += 4;
    char str_count[12];
    sprintf(str_count, "%d", instruction_count);
    Instruction* rbss_load =  create_instruction("loadI", str_count, NULL, "rbss", rsp_load);
    Instruction* jump = create_instruction("jumpI", main_func->function_label, NULL, NULL, rbss_load);
    node->instr = concat_instructions(node->instr, jump);
  }
}

typedef struct sRegList {
  char* reg;
  struct sRegList* next;
} RegList;

RegList* insert_if_not_exists(char* reg, RegList* list) {
  if (reg == NULL)
    return list;
  
  if (strcmp(reg, "rfp") ==  0
    || strcmp(reg, "rsp") ==  0
    || strcmp(reg, "rpc") ==  0
    || strcmp(reg, "rbss") ==  0
    || reg[0] != 'r'
  ) {
    return list;
  }

  if (list == NULL) {
    RegList* item = (RegList*) malloc(sizeof(RegList));
    item->reg = reg;
    item->next = NULL;
    return item;
  }

  bool found = false;
  RegList* item = list;
  while (found == false && item != NULL) {
    if (strcmp(reg, item->reg) ==  0) {
      found = true;
    }
    item = item->next;
  }

  if (found == false) {
    RegList* new_item = (RegList*) malloc(sizeof(RegList));
    new_item->reg = reg;
    new_item->next = list;
    return new_item;
  }
  return list;
}

void complete_holes(Instruction* code, Table_Stack* scopes) {
  if (code == NULL)
    return;

  Instruction* loads = NULL;
  Instruction* stores = NULL;
  Instruction* pre_load = NULL;
  int return_offset = 0;
  Instruction* prev = NULL;
  Instruction* instr = code;

  while (instr != NULL) {
    if (strcmp(instr->opcode, "$store$") == 0) {
      
      Instruction* search = instr->previous;
      RegList* regs = NULL;
      
      while (search != NULL) {
        regs = insert_if_not_exists(search->operand1, regs);
        regs = insert_if_not_exists(search->operand2, regs);
        regs = insert_if_not_exists(search->operand3, regs);
        search = search->previous;
      }

      RegList* item = regs;
      int i = 1;
      while (item != NULL) {
        char stack_offset[12];
        sprintf(stack_offset, "%d", return_offset + 4*(i-1)); // TODO, This size will change dependning on size of return type
        stores = create_instruction("storeAI", item->reg, "rsp", stack_offset, stores);
        loads = create_instruction("loadAI", "rsp", stack_offset, item->reg, loads);
        
        i += 1;
        item = item->next;
      }

      if(stores != NULL){
        char stack_offset[12];
        sprintf(stack_offset, "%d", 4*(i - 1));
        stores = create_instruction("addI", "rsp", stack_offset, "rsp", stores);
        pre_load = create_instruction("subI", "rsp", stack_offset, "rsp", NULL);
        
        prev->previous = stores;
      
        while(stores->previous != NULL){
          stores = stores->previous;
        }
        stores->previous = instr->previous;
      }
      else{
        instr->opcode = strdup("nop");
      }
    }
    prev = instr;
    instr = instr->previous;
  }

  prev = NULL;
  instr = code;

  while(instr != NULL){
      if(strcmp(instr->opcode, "$load$") == 0){
      
      if(loads != NULL){
        prev->previous = loads;
      
        while(loads->previous != NULL){
          loads = loads->previous;
        }
        loads->previous = pre_load;
        pre_load->previous = instr->previous;
      }
      else{
        instr->opcode = strdup("nop");
      }
    }

    prev = instr;
    instr = instr->previous;
  }
}


