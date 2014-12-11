//adjacent.c 
//compile using "gcc -Werror -Wall -O3 adjacent.c -o adjacent"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "adjacent.h"

int main(int Argc, char **Argv)
{
  //Test to make sure input is given
  if(Argc < 2)
  {
    printf("Error, no input given.\n");
    return(0);
  }
  FILE * fp = fopen(Argv[1], "r");
  if(fp == NULL)
  {
    return 0;
  }
  int numvert, numedges, i;
  int tempnode, tempx, tempy;
  int tempnode1, tempnode2;
  Node * list = NULL;
  //get the first line with the number of vertices and edges
  fscanf(fp, " %d %d\n", &numvert, &numedges);
  int *dist[numvert];
  for (i = 0; i < numvert; i++)
    dist[i] = (int*)malloc(2 * sizeof(int));
  //scan through all the nodes with there points, creating a linked list of them
  for(i = 0; i < numvert; i++)
  {
    fscanf(fp, " %d %d %d\n", &tempnode, &tempx, &tempy);
    list = Node_Insert(list, tempnode, tempx, tempy);
    dist[i][0] = tempx;
    dist[i][1] = tempy;
  }
  Node * temp;
  temp = list;
  //now scan for all the intersections, and create a linked list of them
  for(i = 0; i < numedges; i++)
  {
    fscanf(fp, " %d %d\n", &tempnode1, &tempnode2);
      //add the link to the first node
    if(temp -> next != NULL && tempnode1 != temp -> node)
    {
     temp = temp -> next;
   }
   if(tempnode1 != temp -> node)
   {
     temp = list;
     while(tempnode1 != temp -> node)
     {
       temp = temp -> next;
     }
   }
   Neighbor_Insert(temp, temp -> next_n, tempnode2);
      //add the link to the second node
   if(temp -> next != NULL && tempnode2 != temp -> node)
     temp = temp -> next;
   if(tempnode2 != temp -> node)
   {
     temp = list;
     while(tempnode2 != temp -> node)
     {
       temp = temp -> next;
     }
   }
   Neighbor_Insert(temp, temp -> next_n, tempnode1);
 }
 temp = list;
 Neighbor * temp_n;
  //print all the items
 while(temp != NULL)
 {
      //find the first node with a neighbor
  if (temp -> next_n != NULL)
  {
   temp_n = temp -> next_n;
   printf("%d:", temp -> node);
	  //print the remaining neighbors
   while(temp_n -> next != NULL)
   {
     printf(" %d", temp_n -> node);
     temp_n = temp_n -> next;
   }
   printf(" %d", temp_n -> node);
 }
 printf("\n");
  temp = temp -> next; 
  if (temp == NULL)
  {
    printf("temp set to null");
  }
}   
printf("test1");
int ** graph = (int**)malloc(numvert * sizeof(int *));
for(i = 0; i < numvert; i++)
  graph[i] = (int *)calloc(numvert, sizeof(int));
temp = list;
printf("test");
while(temp != NULL)
{
  if (temp -> next_n != NULL)
  {
   temp_n = temp -> next_n;
   while(temp_n -> next != NULL)
   {
     graph[(temp -> node) - 1][(temp_n -> node) - 1] = Distance(temp->x, temp->y, dist[(temp_n -> node) - 1][0],dist[(temp_n -> node) - 1][1]);
     temp_n = temp_n -> next;
   }
   graph[(temp -> node) - 1][(temp_n -> node) - 1] = Distance(temp->x, temp->y, dist[(temp_n -> node) - 1][0],dist[(temp_n -> node) - 1][1]);
 }
 temp = temp -> next;
}
printf("Graph: %d", graph[0][0]);
  //destroy the list
Destroy_List(list);
return(0);
}

Node * Node_Create(int node_num, int x_val, int y_val)
{
  Node * ln = malloc(sizeof(Node));
  ln -> node = node_num;
  ln -> x = x_val;
  ln -> y = y_val;
  ln -> next = NULL;
  ln -> next_n = NULL;
  return ln;
}

Neighbor * Neighbor_Create(int node_num)
{
  Neighbor * ln = malloc(sizeof(Neighbor));
  ln -> node = node_num;
  ln -> next = NULL;
  return ln;
}

Node * Node_Insert(Node * head, int node_num, int x_val, int y_val)
{
  //insert nodes, order doesn't matter
  Node * current;
  Node * temp;
  if(head == NULL)
  {
    current = Node_Create(node_num, x_val, y_val);
    return(current);
  }
  //move in the linked list to where it needs to be
  else
  {
    current = head;
    while(current -> next != NULL)
    {
     current = current -> next;
   }
   temp = Node_Create(node_num, x_val, y_val);
   current -> next = temp;
 }
 return head;
}

void Neighbor_Insert(Node * main, Neighbor * head, int node_num)
{
  Neighbor * current;
  if(head == NULL)
  {
    main -> next_n = Neighbor_Create(node_num);
  }
  //move through the linked list
  else
  {
    current = head;
    while(current -> next != NULL)
    {
     current = current -> next;
   }
   current -> next = Neighbor_Create(node_num);
 }
}

void Destroy_List(Node * main)
{
  if(main -> next != NULL)
  {
    Destroy_List(main -> next);
  }
  Destroy_Neighbors(main -> next_n);
  free(main);
}

void Destroy_Neighbors(Neighbor * head)
{
  if(head -> next != NULL)
  {
    Destroy_Neighbors(head -> next);
  }
  free(head);
}

int Distance(int x1, int y1, int x2, int y2)
{
  int p1, p2, value;
  p1 = pow(x2-x1,2);
  p2 = pow(y2-y1,2);
  value = sqrt(p1 + p1);
  return value;
}
