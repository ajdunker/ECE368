// unhuff.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhuff.h"

int main(int Argc, char **Argv)
{
	if(Argc < 2)
	{	
		printf("Error, no input given.\n");
		return(0);
	}
	//create new memory for .unhuff string
	char outfile[255];
	strcpy(outfile, Argv[1]);
	strcat(outfile, ".unhuff");
	decode(Argv[1], outfile);
	return(0);
}

TreeNode * readHeader(FILE * infptr)
{
	int done = 0;
	unsigned char whichbit = 0;
	unsigned char curbyte = 0;
	unsigned char onebit = 0;
	WeightNode * head = NULL;
	while(done == 0)
	{
		readBit(infptr, &onebit, &whichbit, &curbyte);
		if(onebit == 1)
		{
	  //a leaf node, get 7 move bits
			int bitcount;
			unsigned char value = 0;
			for(bitcount = 0; bitcount < 7; bitcount++)
			{
	      value <<= 1; //shift left one
	      readBit(infptr, &onebit, &whichbit, &curbyte);
	      value |= onebit;
	  }
	  //push a tree node into a list
	  TreeNode * tn = TreeNode_create(value, 0);
	  WeightNode * wn = WeightNode_create(tn);
	  head = WeightNode_insert(head, wn);
	}
	else
	{
		if (head == NULL)
			printf("ERROR");
		if((head -> next) == NULL)
		{
			done = 1;
		}
		else
		{
			head = MergeWeightNode(head);
		}
	}
}
TreeNode * root = head -> tree_ptr;
free(head);
return(root);
}

int decode(char * infile, char * outfile)
{
	FILE * infptr = fopen(infile, "r");
	//error checking
	if (infptr == NULL)
	{
		printf("ERROR\nFile %s not opened.\n", infile);
		return 0;
	}
	//make sure it's a .huff file
	if (endswith(".huff", infile) == 0)
	{
		printf("ERROR, input file does not end with .huff extension\n");
		return 0;
	}
	//read the header
	TreeNode * root = readHeader(infptr);
	unsigned int numChar = 0;
	fread(&numChar, sizeof(unsigned int), 1, infptr);
	//make sure the number of characters is greater than 0
	if(numChar < 0)
	{
		printf("numChar less than 0\n");
		return(0);
	}
	unsigned char newline;
	//read the newline character inserted after the header
	fread(&newline, sizeof(unsigned char), 1, infptr);
	if(newline != '\n')
	{
		printf("ERROR! Newline\n");
		return(0);
	}
	unsigned char whichbit = 0;
	unsigned char onebit = 0;
	unsigned char curbyte = 0;
	//open the new file for writing too
	FILE * outfptr = fopen(outfile, "w");
	while(numChar != 0)
	{
		TreeNode * tn = root;
		while ((tn -> left) != NULL)
		{
	  //tn is not a leaf node
			readBit(infptr, &onebit, &whichbit, &curbyte);
			if(onebit == 0)
				tn = tn -> left;
			else
				tn = tn -> right;
		}
		fprintf(outfptr, "%c", tn -> character);
		numChar--;
	}
	//destroy the tree
	Tree_destroy(root);
	//close the files
	fclose(infptr);
	fclose(outfptr);
	printf("File successfully unhuffed!\n");
	return(0);
}
////////////////////////////////////////////////////////////////////////////
// Linked list trees
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
	//return error if given node is NULL
	if (wn == NULL)
	{
		printf("ERROR! wn is NULL\n");
		return NULL;
	}
	if ((wn -> next) != NULL)
	{
		printf("ERROR! wn -> next is not NULL\n");
	}
	//head is null, this will be new head
	if(head == NULL)
	{
		return wn;
	}
	//tack on to the end
	wn -> next = head;
	return wn;

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

//merge 2 nodes
WeightNode * MergeWeightNode(WeightNode * head)
{
	//grab firs ttwo nodes
	WeightNode * second = head -> next;
	WeightNode * third = second -> next;
	//get their trees
	TreeNode * tn1 = head -> tree_ptr;
	TreeNode * tn2 = second -> tree_ptr;
	//free them
	free(head);
	free(second);
	head = third;
	TreeNode * fin;
	//merge the two trees
	fin = Tree_merge(tn2, tn1);
	//create new weight node and reinsert it
	WeightNode * wn = WeightNode_create(fin);
	head = WeightNode_insert(head, wn);
	return head;
}

//read the bit into something usable
int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte)
{
	int ret = 1;
	if((*whichbit) == 0)
	{
		ret = fread(curbyte, sizeof(unsigned char), 1, fptr);
	}
	if(ret != 1)
	{
      //read fail
		return -1;
	}
	unsigned char temp = (*curbyte) >> (7 - (*whichbit));
	temp = temp & 0X01;
	*whichbit = ((*whichbit) + 1) % 8;
	*bit = temp;
	return 0;
}

//check if one string, ends in another string used to check for .huff / .unhuff
int endswith(const char* end, const char* what)
{
	int length1 = strlen(end);
	int length2 = strlen(what);
	if (length1 > length2)
		return 0;

	return strcmp(end, what + (length2 - length1)) == 0;
}
