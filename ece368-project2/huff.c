// Huff.c
#include <stdio.h>
#include <stdlib.h>
#include "huff.h"

int main(int Argc, char **Argv)
{
  //Test to make sure input is given
  struct freq_list * root;
  root = malloc(sizeof(struct freq_list));
  if(Argc < 2)
    {
      printf("Error, no input given.\n");
      return(0);
    }
  frequency_count(Argv[1], root);
  struct freq_list * place;
  struct freq_list * temp;
  place = root;
  while(place!=NULL)
    {
      printf("Frequency of %c = %d\n", place -> char_val, place -> freq);
      place = place -> next;
    }

  place = root;
  while (place != NULL)
    {
      temp = place;
      place = place -> next;
      free(temp);
    }
  free(root);
  return(0);
}


void frequency_count(char *Filename, struct freq_list * root)
{
  int frequency[127];
  int ch;
  int node_created = 0;
  FILE * fp = fopen(Filename, "r");
  for (ch = 0; ch < 127; ch++)
    {
      frequency[ch] = 0;
    }
  while (1)
    {
      ch = fgetc(fp);
      if (ch == EOF)
	  break;
      frequency[ch]++;
    }
  fclose(fp);
  //struct * root;
  struct freq_list * place;
  //root = malloc(sizeof(struct freq_list));
  place = root;
  int i;
  for(i = 0; i < 127; i++)
    {
      if (frequency[i] > 0)
	{
	  place -> freq = frequency[i];
	  place -> char_val = i;
	  place -> next = malloc(sizeof(struct freq_list));
	  place = place -> next;
	  node_created++;
	}
    }
  place = root;
  for(i = 0; i < node_created - 1; i++)
    {
      place = place -> next;
    }
  place -> next = NULL;
  free(place -> next);
}
