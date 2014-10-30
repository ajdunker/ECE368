// unhuff.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unhuff.h"
#define QUEUE 0
#define STACK 1
#define SORTED 2
#define ENCODEMODE 0
#define DECODEMODE 1


int main(int Argc, char **Argv)
{
	if(Argc < 2)
	{	
		printf("Error, no input given.\n");
		return(0);
	}

  	//char * outfile = strcat(Argv[1], ".unhuff");
  	//char * infile = Argv[1];
  	//printf("infile: %s\noutfile: %s\n", infile, outfile);
	decode("test.txt.huff", "test.txt.huff.unhuff");
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
			head = WeightNode_insert(head, wn, STACK);
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
				head = MergeWeightNode(head, DECODEMODE);
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
	if (infptr == NULL)
	{
		printf("ERROR\nFile %s not opened.\n", infile);
		return 0;
	}
	TreeNode * root = readHeader(infptr);
	Tree_print(root, 0);
	unsigned int numChar = 0;
	fread(&numChar, sizeof(unsigned int), 1, infptr);
	printf("numChar = %d\n", numChar);
	if(numChar < 0)
	{
		printf("numChar less than 0\n");
		return(0);
	}
	unsigned char newline;
	fread(&newline, sizeof(unsigned char), 1, infptr);
	if(newline != '\n')
	{
		printf("ERROR! Newline\n");
		return(0);
	}
	unsigned char whichbit = 0;
	unsigned char onebit = 0;
	unsigned char curbyte = 0;
	FILE * outfptr = fopen(outfile, "w");
	printf("test\n");
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
		printf("%c", tn -> character);
		fprintf(outfptr, "%c", tn -> character);
		numChar--;
	}
	Tree_destroy(root);
	fclose(infptr);
	fclose(outfptr);
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

WeightNode * WeightNode_insert(WeightNode * head, WeightNode * wn, int mode)
{
	if (wn == NULL)
	{
		printf("ERROR! wn is NULL\n");
		return NULL;
	}
	if ((wn -> next) != NULL)
	{
		printf("ERROR! wn -> next is not NULL\n");
	}
	if(head == NULL)
	{
		return wn;
	}
	if (mode == STACK)
	{
		wn -> next = head;
		return wn;
	}
	if (mode == QUEUE)
	{
		head -> next = WeightNode_insert(head -> next, wn, mode);
		return head;
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
	head -> next = WeightNode_insert(head -> next, wn, mode);
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

WeightNode * MergeWeightNode(WeightNode * head, int endec)
{
	WeightNode * second = head -> next;
	WeightNode * third = second -> next;
	TreeNode * tn1 = head -> tree_ptr;
	TreeNode * tn2 = second -> tree_ptr;
	free(head);
	free(second);
	head = third;
	TreeNode * fin;
	if (endec == ENCODEMODE)
	{
		fin = Tree_merge(tn1, tn2);
	}
	else
	{
		fin = Tree_merge(tn2, tn1);
	}
	WeightNode * wn = WeightNode_create(fin);
	if (endec == ENCODEMODE)
	{
		head = WeightNode_insert(head, wn, SORTED);
	}
	else
	{
		head = WeightNode_insert(head, wn, STACK);
	}
	return head;
}

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

// post - order
void Tree_print ( TreeNode * tn , int level )
{
	if ( tn == NULL )
	{
		return;
	}
TreeNode * lc = tn -> left ; // left child
TreeNode * rc = tn -> right ; // right child

Tree_print ( lc , level + 1) ;
Tree_print ( rc , level + 1) ;
int depth ;
for ( depth = 0; depth < level ; depth ++)
{
	printf ("   ") ;
}
printf (" freq = %d" , tn -> weight);
if ((lc == NULL) && (rc == NULL))
{
// a leaf node
	printf (" value = %d , ’%c’" , tn -> character , tn -> character ) ;
}
printf ("\n");

}