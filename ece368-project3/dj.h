#include <stdio.h>
#include <stdlib.h>

typedef int bool;
#define true 1
#define false 0

int Distance(int x1, int y1, int x2, int y2);
int minDistance(int dist[], bool sptSet[], int V);
void dijkstra(int **graph, int src, int V, int target);
