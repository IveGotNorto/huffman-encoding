#include "huffman_tree_node.h"

void addRec(PriNode *priHead, PriNode *newNode) {

	if((priHead->next == NULL)) {

		priHead->next = newNode;

	} else if((priHead->next->value->weight) >= (newNode->value->weight)) {

		newNode->next = priHead->next;
		priHead->next = newNode;

	} else {

		addRec(priHead->next, newNode);

	}

}

void add(PriNode **priHead, HuffNode *huffNode) {

	PriNode *newPriNode = malloc(sizeof(PriNode));
	PriNode *holdHead = *priHead;
	newPriNode->value = huffNode;		
	newPriNode->next = NULL;
	
	if(!(*priHead)) {

		*priHead = newPriNode;

	} else if(((*priHead)->value->weight) >= (newPriNode->value->weight)) {

		newPriNode->next = *priHead;
		*priHead = newPriNode;

	} else {

		addRec(holdHead, newPriNode);

	}

}

struct HuffmanTreeNode *pop(PriNode **head) {

	HuffNode *hold = (*head)->value;
	PriNode *oldHead;

	if(head != NULL) {
		oldHead = *head;
		oldHead->value = NULL;
		*head = (*head)->next;
		free(oldHead);
	}

	return hold;	

}

struct HuffmanTreeNode *merge(HuffNode *node1, HuffNode *node2) {

	HuffNode *newNode = malloc(sizeof(HuffNode));		
	newNode->left = node1;
	newNode->right = node2;
	newNode->weight = (node1->weight) + (node2->weight);

	for(int i=0;i<17;i++) {
		(newNode->value[i]) = ((node1->value[i]) | (node2->value[i]));
	}

	return newNode;

}

struct HuffmanTreeNode *buildLeaf(int charCount, int asciiValue) {

	int byteOffset = asciiValue / 8;
	int bytePosition = asciiValue % 8; 

	HuffNode *newNode = malloc(sizeof(HuffNode));	
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->weight = charCount;
	newNode->value[byteOffset] = 1 << bytePosition; 

	return newNode;

}

int isLeaf(HuffNode *node) {

	return !(node->left) && !(node->right);

}

void buildTree(FILE *finCharCount, PriNode **headPriNode) {

	int numValues = 128;
	int *j = malloc(numValues * sizeof(int));
	HuffNode *newHuffNode = NULL;
	HuffNode *newHuffNode2 = NULL;
	HuffNode *holdMerge = NULL;

	for(int i=0;i<numValues;i++) {
		if(finCharCount != NULL && i == 0) {
			fread(j, sizeof(int), numValues, finCharCount);	
		}
		newHuffNode = buildLeaf(j[i], i);

		add(headPriNode, newHuffNode);
	}

	// Add EOF to Priority Queue
	newHuffNode = buildLeaf(1, 128);
	add(headPriNode, newHuffNode);

	while((*headPriNode)->next != NULL) {
		newHuffNode = pop(headPriNode);
		newHuffNode2 = pop(headPriNode);
		holdMerge = merge(newHuffNode, newHuffNode2);	
		add(headPriNode, holdMerge);
	}

	free(j);

}

void freeTreeRec(HuffNode *root) {

	if(root == NULL) {
		return;
	} else {

		if(root->left != NULL) {
			freeTreeRec(root->left);
		}

		if(root->right != NULL) {
			freeTreeRec(root->right);
		}

		if(isLeaf(root)) {
			free(root);
			root = NULL;
		}

	} 

}

void freeTree(HuffNode *root) {

	HuffNode *clone = root;
	if(isLeaf(root)) {
		free(root);
		root = NULL;
	} else {
		freeTreeRec(clone);
	}

}

