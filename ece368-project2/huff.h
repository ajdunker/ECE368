#include <stdio.h>
#include <stdlib.h>

struct freq_list
{
  int freq;
  int char_val;
  struct freq_list *next;
};

void frequency_count(char *Filename, struct freq_list * root);




