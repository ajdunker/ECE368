#include <stdio.h>
#include <stdlib.h>

typedef struct node_list
{
  int node;
  int x;
  int y;
  struct node_list *next;
  struct neighbor_list *next_n;
} Node;

typedef struct neighbor_list
{
  int node;
  struct neighbor_list *next;
} Neighbor;

Node * Node_Create(int node_num, int x_val, int y_val);
Node * Node_Insert(Node * head, int node_num, int x_val, int y_val);
Neighbor * Neighbor_Create(int node_num);
void Neighbor_Insert(Node * main, Neighbor * head, int node_num);
void Destroy_List(Node * main);
void Destroy_Neighbors(Neighbor * head);
int Distance(int x1, int y1, int x2, int y2);
