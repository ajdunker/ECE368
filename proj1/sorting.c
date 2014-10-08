#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

long *Load_File (char *Filename, int *Size)
{
  FILE * fil;
  fil = fopen(Filename, "r");
  long reader;
  fscanf(fil, "%ld", &reader);
  *Size = reader;
  long * arr = malloc((*Size) * sizeof(long));
  rewind(fil);
  int i = 0;
  while(fscanf(fil, "%ld", &reader) == 1)
    {
      if(i != 0)
	{
	  arr[i-1] = reader;
	}
      i++;
    }
  fclose(fil);
  return arr;
}
int Save_File (char *Filename, long *Array, int Size) 
{
  FILE * fil = fopen(Filename, "w");
  int i, printed = 0;
  for(i = 0; i <= Size; i++)
    {
      if(i == 0)
	{
	  fprintf(fil, "%d\n", Size);
	}
      else
	{
	  fprintf(fil, "%ld\n", Array[i - 1]);
	  printed++;
	}
    }
  fclose(fil);
  return printed;
}
void Shell_Insertion_Sort (long *Array, int Size, double *NComp, double *NMove)
{
  int q = 1, p = 0;
  while(q < Size)
    {
      q = q * 2;
      p++;
    }
  int array_size = (p * (p + 1)) / 2;
  q = q / 2;
  long * seq_array = malloc(array_size * sizeof(long));
  int i, j, iter = 0;
  long temp, num = 1;
  for(i = 1; i <= p; i++)
    {
      temp = num;
      for(j = 1; j <= i; j++)
	{
	  if (num < Size)
	    {
	      seq_array[iter] = num;
	      num = num * 3 / 2;
	      iter++;
	    }
	}
      num = temp * 2;
    }
  qsort(seq_array, iter, sizeof(long), cmpfunc);
  int x, y, z, gap;
  for(x = 0; x < iter; x++)
    {
      gap = (int) seq_array[x];
      for(y = gap; y < Size; y++)
	{
	  temp = Array[y];
	  *NMove = *NMove + 1.0;
	  *NComp = *NComp + 1.0;
	  for(z = y; z >= gap && temp < Array[z - gap]; z -= gap )
	    {
	      Array[z] = Array[z - gap];
	      *NMove = *NMove + 1.0;
	    }
	  Array[z] = temp;
	  *NMove = *NMove + 1.0;
	}
    }
  free(seq_array);
}
void Improved_Bubble_Sort (long *Array, int Size, double *NComp, double *NMove)
{
  int i, j, gap, swapped = 1;
  double temp;
  gap = Size;
  while (gap > 1 || swapped == 1)
    {
      gap = gap * 10 / 13;
      if (gap == 9 || gap == 10)
	{
	  gap = 11;
	}
      if (gap < 1)
	{
	  gap = 1;
	}
      swapped = 0;
      for(i = 0, j = gap; j < Size; i++, j++)
	{
	  *NComp = *NComp + 1.0;
	  if(Array[i] > Array[j])
	    {
	      *NMove = *NMove + 3.0;
	      temp = Array[i];
	      Array[i] = Array[j];
	      Array[j] = temp;
	      swapped = 1;
	    }
	}
    }
}
void Save_Seq1 (char *Filename, int N)
{
  int q = 1, p = 0;
  while(q < N)
    {
      q = q * 2;
      p++;
    }
  int array_size = (p * (p + 1)) / 2;
  q = q / 2;
  long * seq_array = malloc(array_size * sizeof(long));
  int i, j, iter = 0;
  long temp, gap = 1;
  for(i = 1; i <= p; i++)
    {
      temp = gap;
      for(j = 1; j <= i; j++)
	{
	  if (gap < N)
	    {
	    seq_array[iter] = gap;
	    gap = gap * 3 / 2;
	    iter++;
	    }
	}
      gap = temp * 2;
    }
  qsort(seq_array, iter, sizeof(long), cmpfunc);
  FILE * fil = fopen(Filename, "w");
  for( i = 0; i < iter; i++)
    {
      fprintf(fil, "%ld\n", seq_array[i]);
    }
  free(seq_array);
  fclose(fil);
}
void Save_Seq2 (char *Filename, int N)
{
  double gap_check = 1.3;
  int iter = 1;
  while((long)(N / gap_check) > 1)
    {
      gap_check = gap_check * 1.3;
      iter++;
    }
  long * seq_array = malloc(iter * sizeof(long));
  double gap = 1;
  int i = 0;
  int array_i = 0;
  do
    {
     gap = gap * 1.3;
      if((long)(N / gap) != seq_array[array_i-1])
	{
	  if((long)(N / gap) == 9 || (long)(N / gap) == 10)
	    {
	      seq_array[array_i] = 11;
	    }
	  else
	    {
	      seq_array[array_i] = N / gap;
	    }
	  array_i++;
	}
      i++;
    } while((long)(N / gap) != 1);
  FILE * fp = fopen(Filename, "w");
  int z;
  for( z = 0; z < array_i; z++)
    {
      fprintf(fp,"%ld\n", seq_array[z]);
    }
  free(seq_array);
  fclose(fp);
}
int cmpfunc (const void * a, const void * b)
{
  return ( *(int*)b - *(int*)a );
}
