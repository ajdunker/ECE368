#include <stdio.h>
#include <stdlib.h>
#include "sorting.h"

long *Load_File (char *Filename, int *Size)
{
  FILE *fil;
  fil = fopen(Filename, "r");
  long temp;
  fscanf(fil, "%ld", &temp);
  *Size = temp;
  long * arr = malloc((*Size) * sizeof(long));
  rewind(fil);
  int i = 0;
  while(fscanf(fil, "%ld", &temp) == 1)
    {
      if(i != 0 )
	{
	  arr[i-1] = temp;
	}
      i++;
    }
  fclose(fil);
  return arr;
}
int Save_File (char *Filename, long *Array, int Size) 
{
  FILE *fp = fopen(Filename, "w");
  int n;
  for( n = 0; n< Size; n++)
    {
      if(n==0)
	{
	  fprintf(fp,"%d\n", Size);
	}
      else
	{
	  fprintf(fp,"%ld\n", Array[n]);
	}
    }
  fclose(fp);
  return Size;
}
void Shell_Insertion_Sort (long *Array, int Size, double *NComp, double *NMove)
{
  long * seq_array = malloc(Size*sizeof(long));
  int q = 1;
  int p = 0;
  while(q < Size)
    {
      q = q * 2;
      p++;
    }
  q = q/2;
  int i;
  int a;
  long num = 1;
  long temp;
  int iter = 0;
  for(i = 1; i <= p; i++)
    {
      temp = num;
      for(a = 1; a <= i; a++)
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
  int x;
  int y;
  int z;
  int gap;
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
      if (gap == 9 || gap == 10) gap = 11;
      if (gap < 1) gap = 1;
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
  long * seq_array = malloc(N * sizeof(long));
  int q = 1;
  int p = 0;
  while(q < N)
    {
      q = q * 2;
      p++;
    }
  q = q/2;
  int i;
  int a;
  long num = 1;
  long temp;
  int iter = 0;
  for(i = 1; i <= p; i++)
    {
      temp = num;
      for(a = 1; a <= i; a++)
	{
	  if (num < N)
	    {
	    seq_array[iter] = num;
	    num = num * 3 / 2;
	    iter++;
	    }
	}
      num = temp * 2;
    }
  qsort(seq_array, iter, sizeof(long), cmpfunc);
  FILE *fp = fopen(Filename, "w");
  int z;
  for( z = 0; z < iter; z++)
    {
      fprintf(fp,"%ld\n", seq_array[z]);
    }
  free(seq_array);
  fclose(fp);
}
void Save_Seq2 (char *Filename, int N)
{
  long * seq_array = malloc(N * sizeof(long));
  double gap = 1;
  int i = 0;
  int array_i = 0;
  do
    {
     gap = gap * 1.3;
      if( i != 0 && (long)(N / gap) != seq_array[array_i-1])
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

  qsort(seq_array, array_i, sizeof(long), cmpfunc);
  FILE *fp = fopen(Filename, "w");
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
