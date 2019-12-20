#ifndef HUFFMAN_TREE_NODE_H_
#define HUFFMAN_TREE_NODE_H_
#include <stdio.h>
#include <stdlib.h>

typedef struct HuffmanTreeNode {

	struct HuffmanTreeNode *left;
	struct HuffmanTreeNode *right;
	unsigned char value[17];
	int weight;

} HuffNode;

typedef struct PriorityQueueNode {

	struct HuffmanTreeNode *value;
	struct PriorityQueueNode *next;

} PriNode; 

void add(PriNode **priHead, HuffNode *huffNode);

struct HuffmanTreeNode *pop(PriNode **head);

struct HuffmanTreeNode *merge(HuffNode *node1, HuffNode *node2);

struct HuffmanTreeNode *buildLeaf(int charCount, int asciiValue);

int isLeaf(HuffNode *node);

void buildTree(FILE *finCharCount, PriNode **headPriNode);

void freeTree(HuffNode *root);

void freePriority(PriNode **head);

#endif
