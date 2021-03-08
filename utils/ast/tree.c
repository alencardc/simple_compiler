#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

Node* create_node(LexValue *data, const char *label) {
  Node *node;
  node = (Node*) malloc(sizeof(Node));
  if (node == NULL) {
    return NULL;
  }

  node->label = strdup(label);
  node->data = data;
  node->children_amount = 0;
  node->children = NULL;

  return node;
}

Node* append_child(Node *parent, Node *child) {
  if (parent == NULL || child == NULL) {
    return NULL;
  }

  parent->children_amount += 1;
  parent->children = realloc(parent->children, parent->children_amount * sizeof(Node));
  parent->children[parent->children_amount - 1] = child;

  return parent;
}

void free_node(Node *node) {
  if (node == NULL) {
    return;
  }

  // Free all sub-tree from children
  for (int i = 0; i < node->children_amount; i++) {
    free_node(node->children[i]);
  }

  if (node->data != NULL) {
    const TokenValueType value_type = node->data->value_type;
    const TokenType token_type = node->data->token_type;
    if (value_type == CHAR_VAL || value_type == STRING_VAL || token_type != TOKEN_LITERAL) {
      free(node->data->token_value.s_value);
    }
    free(node->data); 
  }

  free(node->label);
  free(node->children);
  free(node);
}

void print_debug_node(Node *root) {
  LexValue *data = root->data;
  printf("%p [label=\"%s", root, root->label);
  if (data != NULL) {
    char *token_type;
    switch (data->token_type) {
    case TOKEN_IDENTIFIER:
      token_type = strdup("TOKEN_IDENTIFIER");
      break;
    case TOKEN_LITERAL:
      token_type = strdup("TOKEN_LITERAL");
      break;
    case TOKEN_SPECIAL:
      token_type = strdup("TOKEN_SPECIAL");
      break;
    case TOKEN_COMPOUND_OP:
      token_type = strdup("TOKEN_COMPOUND_OP");
      break;
    default:
      token_type = strdup(" ");
      break;
    }
    switch (data->value_type) {
      case INTEGER_VAL:
        printf("\\n %s\\n INTEGER_VAL\\n Value:%d", token_type, data->token_value.i_val);
        break;
      case BOOL_VAL:
        printf("\\n %s\\n BOOL_VAL\\n Value:%s", token_type, data->token_value.b_val == true ? "true" : "false");
        break;
      case FLOAT_VAL:
        printf("\\n %s\\n FLOAT_VAL\\n Value:%g", token_type, data->token_value.f_val);
        break;
      case STRING_VAL:
        printf("\\n %s\\n STRING_VAL \\n Value:%s", token_type, data->token_value.s_value);
        break;
      case CHAR_VAL:
        printf("\\n %s\\n CHAR_VAL \\n Value:%s", token_type, data->token_value.s_value);
      default:
        printf("\\n %s\\n CHAR_VAL \\n Value:", token_type);
        break;
    }
    free(token_type);
  }
  printf("\"];\n");
}

void export_labels(Node *root) {
  if (root == NULL) {
    return;
  }

  #ifdef DEBUG
    print_debug_node(root);
  #else
    printf("%p [label=\"%s\"];\n", root, root->label);
  #endif

  for (int i = 0; i < root->children_amount; i += 1) {
    export_labels(root->children[i]);
  }
}

void export_relations(Node *root) {
  if (root == NULL) {
    return;
  }

  for (int i = 0; i < root->children_amount; i += 1) {
    // Quebrar ou nao a linha?
    printf("%p, %p\n", root, root->children[i]);
    export_relations(root->children[i]);
  }
}

void exporta(void *arvore) {
  export_relations((Node*) arvore);
  printf("\n");
  export_labels((Node*) arvore);
}

void libera(void *arvore) {
  free_node((Node*)arvore);
}