#include <stdio.h>
#include <stdlib.h>

typedef struct tree_node_t {
  int character;
  int weight;
  struct tree_node_t * left;
  struct tree_node_t * right;
} TreeNode;

typedef struct weightnode {
	struct weightnode * next;
	TreeNode * tree_ptr;
} WeightNode;

typedef struct freq_list
{
  int freq;
  int char_val;
  struct freq_list *next;
} Node;

TreeNode * readHeader(FILE * infptr);

WeightNode * MergeWeightNode(WeightNode * head, int mode);
WeightNode * WeightNode_create(TreeNode * tn);
WeightNode * WeightNode_insert(WeightNode * head, WeightNode * wn, int mode);

void Tree_destroy(TreeNode * node);
TreeNode * TreeNode_create(int char_val, int weight_val);
TreeNode * Tree_merge(TreeNode * tn1, TreeNode * tn2);

int readBit(FILE * fptr, unsigned char * bit, unsigned char * whichbit, unsigned char * curbyte);
int decode(char * infile, char * outfile);
void Tree_print ( TreeNode * tn , int level );