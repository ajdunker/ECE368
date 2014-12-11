//dj.c 
//compile using "gcc -Werror -Wall -O3 -lm adjacent.c -o adjacent"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "dj.h"


int main(int Argc, char **Argv)
{
//Test to make sure input is given
	if(Argc < 3)
	{
		printf("Error, proper input not given.\n");
		return(0);
	}
	FILE * fp = fopen(Argv[1], "r");
	if(fp == NULL)
	{
		return 0;
	}
	FILE * fp2 = fopen(Argv[2], "r");
	if(fp2 == NULL)
	{
		return 0;
	}
	int querynum;
	fscanf(fp2, "%d\n", &querynum);
	int numvert, numedges, i;
	int tempnode, tempx, tempy;
	int tempnode1, tempnode2;
	int tempdist;
//get the first line with the number of vertices and edges
	fscanf(fp, " %d %d\n", &numvert, &numedges);
//allocate arrays for locations and connections
	int **loc = (int **)malloc(sizeof(int*)*numvert);
	int **conn = (int **)malloc(sizeof(int*)*numvert);
	for (i = 0; i < numvert; i++)
	{
		loc[i] = (int*)calloc(2, sizeof(int));
		conn[i] = (int*)calloc(numvert, sizeof(int));
	}
//create the array of locations
	for(i = 0; i < numvert; i++)
	{
		fscanf(fp, " %d %d %d\n", &tempnode, &tempx, &tempy);
		loc[i][0] = tempx;
		loc[i][1] = tempy;
	}
/*//print out all of our vertexes
	for(i = 0; i < numvert; i++)
	{
		printf("%d (%d,%d)\n", i, loc[i][0], loc[i][1]);
	}*/
//read in all of the connections and create a graph
	for(i = 0; i < numedges; i++)
	{
		fscanf(fp, " %d %d\n", &tempnode1, &tempnode2);
		tempdist = Distance(loc[tempnode1][0], loc[tempnode1][1], loc[tempnode2][0], loc[tempnode2][1]);
		conn[tempnode1][tempnode2] = tempdist;
		conn[tempnode2][tempnode1] = tempdist;
	}
/*	for(i = 0; i < numvert; i++)
	{
		printf("%d:\t", i);
		for(j = 0; j < numvert; j++)
		{
			printf("%d ", conn[i][j]);
		}
		printf("\n");
	}*/
	for(i = 0; i < querynum; i++)
	{
		fscanf(fp2, " %d %d\n", &tempnode1, &tempnode2);
		dijkstra(conn, tempnode1, numvert, tempnode2);
	}
//get rid of everything
	for (i = 0; i < numvert; i++)
	{
		free(loc[i]);
		free(conn[i]);
	}
	free(loc);
	free(conn);
	fclose(fp);
	fclose(fp2);
	return(0);
}

//function for calculating the distance between vertexes
int Distance(int x1, int y1, int x2, int y2)
{
	int p1, p2, value;
	p1 = pow(x2-x1,2);
	p2 = pow(y2-y1,2);
	value = sqrt(p1 + p2);
	return value;
}

//return the closest node
int minDistance(int dist[], bool sptSet[], int V)
{
	int v;
	// set as the minimum value
	int min = INT_MAX;
	int min_index = -1;
	//loop through
	for (v = 0; v < V; v++)
		if (sptSet[v] == false && dist[v] <= min)
			min = dist[v], min_index = v;

	return min_index;
}


// function on  
void dijkstra(int **graph, int src, int V, int target)
{
	int i, v, count;
	int dist[V]; //array of the distances
	bool sptSet[V];	//set of vertices that have been visited or not
	int previous[V];
	previous[src] = -1;
 	// set distances to infinite, and all nodes to unvisited
	for (i = 0; i < V; i++)
		dist[i] = INT_MAX, sptSet[i] = false;

 	// distance to itself is always 0
	dist[src] = 0;

 	// short path for all vertices
	for (count = 0; count < V; count++)
	{
   		// pick the minimum distance vertex that hasn't already been visited
		int u = minDistance(dist, sptSet, V);
   		// set the vertex as visited
		sptSet[u] = true;

   		// update the dist values for the right vertexes
		for (v = 0; v < V; v++)
		{
			// only update the vertexes if it meets the following requirements
			if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u]+graph[u][v] < dist[v])
			{
				//update the distance
				dist[v] = dist[u] + graph[u][v];
				//udpate the array of the route it takes
				previous[v] = u;
			}
		}
	}
	//print the distance
    printf("%d\n", dist[target]);
    int u = target;
    int j;
    int array[V];
    //create an array for final output
    for (i=0;i<V;i++)
    {
        array[i] = 0;
    }
    i = 0;
    array[0] = target;
    //parse the array to get the proper order
    while (previous[u] != -1)
    {
        i++;
        array[i] = previous[u];
        u = previous[u];
    }
    //print the final array
    for(j=i;j>=0;j--)
    {
        printf("%d ",array[j]);
    }
    printf("\n");
}
