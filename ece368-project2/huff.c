// Huff.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huff.h"

int main(int Argc, char **Argv)
{
  //Test to make sure input is given
  if(Argc < 2)
    {
      printf("Error, no input given.\n");
      return(0);
    }
  char outfile[255];
  strcpy(outfile, Argv[1]);
  strcat(outfile, ".huff");
  //create an array to store the frequencies
  int freq_array[127];
  int numChar;
  //call the function to create the frequencies
  numChar = frequency_count(Argv[1], freq_array);
  //create a node for the list
  Node * list;
  //build the list
  list = List_build(freq_array);
  if (list == NULL)
    {
      return(0);
    }
  WeightNode * tree_list;
  tree_list = WeightNode_build(list);
  //combine the trees
  while(tree_list -> next != NULL)
    {
      //grab the second and third
      WeightNode * second = tree_list -> next;
      WeightNode * third = second -> next;
      //grab the trees
      TreeNode * tn1 = tree_list -> tree_ptr;
      TreeNode * tn2 = second -> tree_ptr;
      //free the weight nodes that have been combined
      free(tree_list);
      free(second);
      tree_list = third;
      //merge the trees
      TreeNode * fin = Tree_merge(tn1, tn2);
      WeightNode * wn = WeightNode_create(fin);
      //insert the new weight node back in
      tree_list = WeightNode_insert(tree_list, wn);
    }
  List_destroy(list); //destroy list
  TreeNode * root = tree_list -> tree_ptr; //don't need the tree_list anymore
  free(tree_list);
  //build the code book
  int numRow = Tree_leaf(root);
  int numCol = Tree_height(root);
  numCol++;
  int ** codebook = malloc(sizeof(int*) * numRow);
  int row;
  for(row = 0; row < numRow; row++)
    {
      codebook[row] = malloc(sizeof(int) * numCol);
      int col;
      for(col = 0; col < numCol; col++)
	{
	  codebook[row][col] = -1;
	}
    }
  buildCodeBook(root, codebook);
  int mapping[127];
  int ind;
  for(ind = 0; ind < 127; ind++)
    {
      mapping[ind] = -1;
      int ind2;
      for(ind2 = 0; ind2 < numRow; ind2 ++)
	{
	  if(codebook[ind2][0] == ind)
	    {
	      mapping[ind] = ind2;
	    }
	}
    }
  Tree_header(root, numChar,outfile);
  compress(Argv[1], outfile, codebook, mapping);
  for ( ind = 0; ind < numRow ; ind ++)
    {
      free (codebook[ind]);
    }
  free (codebook);
  Tree_destroy(root);
  return(0);
}

int frequency_count(char *Filename, int * frequency)
{
  int ch;
  int numChar = 0;
  FILE * fp = fopen(Filename, "r"); //open file
  for (ch = 0; ch < 127; ch++) //set every value to 0
    {
      frequency[ch] = 0;
    }
  while (1) //loop through the file finding every character
    {
      ch = fgetc(fp);
      if (ch == EOF)
	{
	  break; //break if at the end of the file
	}
      frequency[ch]++; //increment the corresponding character
      numChar++;
    }
  fclose(fp); //close the file
  return(numChar);
}

////////////////////////////////////////////////////////////////////////////
// Build a linked list of the frequencies
////////////////////////////////////////////////////////////////////////////
Node * List_build(int * frequencies)
{
  Node * ln;
  int j = 0;
  //find the first character that has a frequency
  while(frequencies[j] == 0)
    {
      j++;
    }
  //create the list node
  ln = List_create(j, frequencies[j]);
  j++;
  int i = 0;
  i = j;
  //increment through the frequencies adding each one
  while(i < 127)
    {
      if (frequencies[i] > 0)
	{
	  //add in frequencies ascending
	  ln = List_insert_ascend(ln, i, frequencies[i]);
	}
      i++;
    }
  //return final product
  return ln;
}

//create a list node for the frequencies
Node * List_create(int value, int index)
{
  Node * ln = malloc(sizeof(Node));
  ln -> next = NULL;
  ln -> freq = index;
  ln -> char_val = value;
  return ln;
}

//insert a list node for the frequencies in ascending order
Node * List_insert_ascend(Node * head, int value, int index)
{
  Node * current;
  Node * temp;
  //new value should be at the head of the list
  if (head == NULL || head -> freq >= index)
    {
      current = List_create(value, index);
      current -> next = head;
      return(current);
    }
  //new value shoudl be somewhere else in the list
  else
    {
      current = head;
      //increment through the list
      while(current -> next != NULL && current -> freq < index)
	{
	  current = current -> next;
	}
      temp = List_create(value, index);
      temp -> next = current -> next;
      current -> next = temp;
    }
  return head;
}

//free my list
void List_destroy(Node * head)
{
  if(head != NULL)
    List_destroy(head -> next);
  free(head);
}

////////////////////////////////////////////////////////////////////////////
// Build a linked list of trees
////////////////////////////////////////////////////////////////////////////

//Create a new weight node
WeightNode * WeightNode_create(TreeNode * tn)
{
  WeightNode * wn = malloc(sizeof(WeightNode));
  wn -> next = NULL;
  wn -> tree_ptr = tn;
  return wn;
}

WeightNode * WeightNode_insert(WeightNode * head, WeightNode * wn)
{
  if(head == NULL)
    {
      return wn;
    }
  int weight1 = (head -> tree_ptr) -> weight;
  int weight2 = (wn -> tree_ptr) -> weight;
  if (weight1 > weight2)
    {
      //wn should be inserted to the front
      wn -> next = head;
      return wn;
    }
  //wn should be later in the list
  head -> next = WeightNode_insert(head -> next, wn);
  return head;
}

WeightNode * WeightNode_build(Node * freq_list)
{
  WeightNode * head = NULL;
  //cycle through my list of frequencies and characters and create nodes for a tree
  while(freq_list != NULL)
    {
      TreeNode * tn = TreeNode_create(freq_list -> char_val, freq_list -> freq);
      WeightNode * wn = WeightNode_create(tn);
      head = WeightNode_insert(head, wn);
      freq_list = freq_list -> next;
    }
  return head;
}

//Destroy a tree
void Tree_destroy(TreeNode * node)
{
  if(node == NULL)
    return;
  Tree_destroy(node -> left);
  Tree_destroy(node -> right);
  free(node);
}

//Create new tree nodes
TreeNode * TreeNode_create(int char_val, int weight_val)
{
  TreeNode * node = malloc(sizeof(TreeNode));
  node -> character = char_val;
  node -> weight = weight_val;
  node -> left = NULL;
  node -> right = NULL;
  return node;
}

//merge two trees
TreeNode * Tree_merge(TreeNode * tn1, TreeNode * tn2)
{
  TreeNode * tn = malloc(sizeof(TreeNode));
  tn -> left = tn1;
  tn -> right = tn2;
  tn -> character = 0;
  //make sure to add the weights
  tn -> weight = (tn1 -> weight) + (tn2 -> weight);
  return tn;
}

////////////////////////////////////////////////////////////////////////////
// Build a codebook
////////////////////////////////////////////////////////////////////////////

int Tree_heightHelper(TreeNode * tn, int height)
{
  if (tn == 0)
    return height;
  int left_h = Tree_heightHelper(tn -> left, height + 1);
  int right_h = Tree_heightHelper(tn -> right, height + 1);
  if (left_h < right_h)
    return right_h;
  if (left_h > right_h)
    return left_h;
  return left_h;
}

int Tree_height(TreeNode * tn)
{
  return Tree_heightHelper(tn, 0);
}

void Tree_leafHelper(TreeNode * tn, int * num)
{
  if (tn == 0)
    return;
  TreeNode * lc = tn -> left;
  TreeNode * rc = tn -> right;
  if((lc == NULL) && (rc == NULL))
    {
      (*num)++;
      return;
    }
  Tree_leafHelper(lc, num);
  Tree_leafHelper(rc, num);
}

int Tree_leaf(TreeNode * tn)
{
  int num = 0;
  Tree_leafHelper(tn, &num);
  return num;
}

void buildCodeBookHelper(TreeNode * tn, int ** codebook, int * row, int col)
{
  //error checking
  if (tn == NULL)
    return;
  //check if leaf node
  TreeNode * lc = tn -> left;
  TreeNode * rc = tn -> right;
  if((lc == NULL) && (rc == NULL))
    {
      //finish one code
      codebook[*row][0] = tn -> character;
      (*row)++;
      return;
    }
  if(lc != NULL)
    {
      //enter the subtree to this column
      int numRow = Tree_leaf(lc);
      int ind;
      for(ind = * row; ind < (*row) + numRow; ind++)
	{
	  codebook[ind][col] = 0;
	}
      buildCodeBookHelper(lc, codebook, row, col + 1);
    }
  //now do it for the right side
  if(rc != NULL)
    {
      int numRow = Tree_leaf(rc);
      int ind;
      //move through
      for(ind = *row; ind < (*row) + numRow; ind++)
	{
	  codebook[ind][col] = 1;
	}
      //recursive call
      buildCodeBookHelper(rc, codebook, row, col + 1);
    }
}

void buildCodeBook(TreeNode * root, int ** codebook)
{
  //column starts at 1, since 0 is for the characters
  int row = 0;
  buildCodeBookHelper(root, codebook, &row, 1);
}

////////////////////////////////////////////////////////////////////////////
// Compressing and writing the file
////////////////////////////////////////////////////////////////////////////

int writeBit(FILE * fptr, unsigned char bit, unsigned char * whichbit, unsigned char * curbyte)
{
  if((*whichbit) == 0)
    *curbyte = 0;
  unsigned char temp = bit << (7 - (*whichbit));
  *curbyte |= temp;
  int value = 0;
  if ((*whichbit) == 7)
    {
      int ret;
      ret = fwrite(curbyte, sizeof(unsigned char), 1, fptr);
      if(ret == 1)
	{
	  value = 1;
	}
      else
	{
	  value = -1;
	}
    }
  *whichbit = ((*whichbit) + 1) % 8;
  return value;
}

void char2bits(FILE * outfptr, int ch, unsigned char * whichbit, unsigned char * curbyte)
{
  unsigned char mask = 0x40; //only 7 bits
  while(mask > 0)
    {
      writeBit(outfptr, (ch & mask) == mask, whichbit, curbyte);
      mask >>= 1;
    }
}

void Tree_headerHelper(TreeNode * tn, FILE * outfptr, unsigned char * whichbit, unsigned char * curbyte)
{
  if (tn == NULL)
    {
      return;
    }
  TreeNode * lc = tn -> left;
  TreeNode * rc = tn -> right;
  if((lc == NULL) && (rc == NULL))
    {
      //leaf node
      writeBit(outfptr, 1, whichbit, curbyte);
      char2bits(outfptr, tn -> character, whichbit, curbyte);
      return;
    }
  Tree_headerHelper(lc, outfptr, whichbit, curbyte);
  Tree_headerHelper(rc, outfptr, whichbit, curbyte);
  writeBit(outfptr, 0, whichbit, curbyte);
}

void Tree_header(TreeNode * tn, unsigned int numChar, char * Filename)
{
  FILE * outfptr = fopen(Filename, "w");
  if(outfptr == NULL)
    {
      return;
    }
  unsigned char whichbit = 0;
  unsigned char curbyte = 0;
  Tree_headerHelper(tn, outfptr, &whichbit, &curbyte);
  //write one more 0 to end the header
  writeBit(outfptr, 0, &whichbit, &curbyte);
  padZero(outfptr, &whichbit, &curbyte);
  //write the number of characters
  fwrite(&numChar, sizeof(unsigned int), 1, outfptr);
  //add '\n' at the end of the header
  unsigned char newline = '\n';
  fwrite(&newline, sizeof(unsigned char), 1, outfptr);
  fclose(outfptr);
}

int compress(char * infile, char * outfile, int ** codebook, int * mapping)
{
  FILE * infptr = fopen(infile, "r");
  if(infptr  == NULL)
    {
      printf("ERROR");
      return(0); 
    }

  FILE * outfptr = fopen(outfile, "a"); //apend
  if(outfptr == NULL)
    {
      fclose(outfptr);
      printf("ERROR");
      return(0);
    }
  unsigned char whichbit = 0;
  unsigned char curbyte = 0;
  while(!feof(infptr))
    {
      int onechar = fgetc(infptr);
      if(onechar != EOF)
	{
	  int ind = mapping[onechar];
	  int ind2 = 1;
	  while(codebook[ind][ind2] != -1)
	    {
	      writeBit(outfptr, (codebook[ind][ind2] == 1), &whichbit, &curbyte);
	      ind2++;
	    }
	}
    }
  padZero(outfptr, &whichbit, &curbyte);
  fclose(infptr);
  fclose(outfptr);
  printf("Compressed file written successfully.\n");
  return(0);
}

int padZero(FILE * fptr, unsigned char * whichbit, unsigned char * curbyte)
{
  int rtv = 0;
  while((*whichbit) != 0)
    {
      rtv = writeBit(fptr, 0, whichbit, curbyte);
      if(rtv == -1)
	{
	  return -1;
	}
    }
  return rtv;
}
