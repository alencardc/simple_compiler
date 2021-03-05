#include <stdio.h>
#include <stdlib.h>
#include "../ast.h"


int main() {
  Value vp = {.f_value=0.0f, .i_value=0};
  Value v1 = {.f_value=1.0f, .i_value=1};
  Value v2 = {.f_value=2.0f, .i_value=2};
  Value v3 = {.f_value=3.0f, .i_value=3};
  Value v11 = {.f_value=11.0f, .i_value=11};
  Value v12 = {.f_value=12.0f, .i_value=12};
  Value v21 = {.f_value=21.0f, .i_value=21};
  Node* p = create_node(&vp, "Parent");
  Node* c1 = create_node(&v1, "Node 1");
  Node* c2 = create_node(&v2, "Node 2");
  Node* c3 = create_node(&v3, "Node 3");
  Node* c11 = create_node(&v11, "Node 1.1");
  Node* c12 = create_node(&v12, "Node 1.2");
  Node* c21 = create_node(&v21, "Node 2,1");

  if (!p || !c1 || !c2 || !c3 || !c11 || !c12 || !c21) {
    printf("Error creating node\n");
  }

  append_child(p, c1);
  append_child(p, c2);
  append_child(p, c3);
  append_child(c1, c11);
  append_child(c1, c12);
  append_child(c2, c21);

  printf("Parent: %d\n", p->data->i_value);
  printf("Child 1: %d\n", p->children->data->i_value);
  printf("Child 2: %d\n", p->children->next->data->i_value);
  printf("Child 3: %d\n", p->children->next->next->data->i_value);
  printf("Child 11: %d\n", c1->children->data->i_value);
  printf("Child 12: %d\n", c1->children->next->data->i_value);
  printf("Child 21: %d\n", c2->children->data->i_value);

  free_node(p);
  return 0;
}