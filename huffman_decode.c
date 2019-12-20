#include <stdlib.h>
#include <stdio.h>
#include "huffman_tree_node.h"

void printAscii(FILE *writ, HuffNode *clone, int *End) {

	int i = 0;
	int c = 1;
	int asciiValue = 0;
	unsigned char zero = 0;
	unsigned char bitBuffer = 1;
	int counter = 0;

	while(!(clone->value[i] | zero)) {
		i++;
	}

	while(!(clone->value[i] & bitBuffer)) {
		bitBuffer <<= 1;
		c++;
	}


	if(i != 16) {

		asciiValue = (((i * 8) - 1) + c);
		fputc(asciiValue, writ);

	} else {
		*End = 1;
	}

	
}

void travTree(FILE *writ, HuffNode *root, HuffNode **clone, unsigned char input, unsigned char *bitBuffer, int *position, int *End) {

	while (!isLeaf(*clone) && !(*End)) {

		*bitBuffer = 1 << *position ;

		if(!(*bitBuffer & input)) {
			*clone = (*clone)->left;
		} else {
			*clone = (*clone)->right;
		}

		(*position)--;
		*bitBuffer = 0;

		// Check if we've processed the byte
		// and the off chance that we end at a leaf
		if((*position < 0) && isLeaf(*clone)) {
			printAscii(writ, *clone, End);	
			*clone = root;
			break;
		// or Check if the byte has been processed
		} else if(*position < 0 ) {
			*clone = *clone;
			break;
		}

		if(isLeaf(*clone)) {
			printAscii(writ, *clone, End);	
			*clone = root;
		}

	}	

	*position = 7;
	*bitBuffer = 0;	

}

void decodeFile(FILE *writ, FILE *read, HuffNode *root) {

	unsigned char input = 0; 
	unsigned char  bitBuffer = 0;
	int position = 7;
	int End = 0;
	HuffNode *clone = root;

	while(!End) {
		input = fgetc(read);
		travTree(writ, root, &clone, input, &bitBuffer, &position, &End);
	}

}

int main(int argc, char *argv[]){
	// print usage and quit if we don't have two command-line arguments
	if(argc != 4){
		printf("Usage: ./path_to_charcount ./path_to_encode__out ./path_for_decode_out\n");
		return -1;
	}
	
	// print error and quit if we can't open a binary-read file handler to argv[1]
	FILE *finCharCount = fopen(argv[1],"rb");
	if(!finCharCount){
		printf("Charcount file not found.\n");
		return -2;
	}

	FILE *finTextFile = fopen(argv[2],"r");
	if(!finTextFile){
		printf("Huffman file input not found.\n");
		return -3;
	}

	FILE *outputFile = fopen(argv[3],"wb");
	if(!outputFile){
		printf("Ouput file location not found.\n");
		return -3;
	}

	PriNode *headPriNode = NULL;

	buildTree(finCharCount, &headPriNode);
	

	HuffNode *root = pop(&headPriNode);

	decodeFile(outputFile, finTextFile, root);

	freeTree(root);
	fclose(finCharCount);
	fclose(outputFile);
	fclose(finTextFile);

}
