#include <stdlib.h>
#include <stdio.h>
#include "huffman_tree_node.h"

void checkBuffer(FILE *writ, int asciiValue, unsigned char *bitBuffer, int *counter, int checkEnd) {

	if((asciiValue == 128) && checkEnd) {
		while(*counter < 8) {
			*bitBuffer <<= 1;
			(*counter)++;
		}
	}

	if(*counter == 8) {

		fwrite(bitBuffer, sizeof(unsigned char), 1, writ);
		*counter = 0;
		*bitBuffer = 0;
	} 

}

void travTree(FILE *writ, HuffNode *root, int asciiValue, unsigned char *bitBuffer, int *counter) {

	HuffNode *clone = root;
	int comparison = 1 << (asciiValue % 8);
	int byteOffset = asciiValue / 8;
	int checkEnd = 0;

	while (!isLeaf(clone)) {
		if(((clone->left->value[byteOffset]) & comparison)) {
			checkBuffer(writ, asciiValue, bitBuffer, counter, checkEnd);
			clone = clone->left;
			*bitBuffer <<= 1;
		} else {
			checkBuffer(writ, asciiValue, bitBuffer, counter, checkEnd);
			clone = clone->right;
			*bitBuffer <<= 1;
			*bitBuffer |= 1;	
		}

		(*counter)++;

	}	

	if(asciiValue == 128) {
		checkEnd = 1;
		checkBuffer(writ, asciiValue, bitBuffer, counter, checkEnd);
	}

}

void encodeFile(FILE *writ, FILE *read, HuffNode *root) {

	int holder;
	unsigned char bitBuffer = 0;
	int counter = 0;

	while(holder = fgetc(read)) {
		if(holder == EOF) {
			travTree(writ, root, 128, &bitBuffer, &counter);
			break;	
		} else {
			travTree(writ, root, holder, &bitBuffer, &counter);
		}

	}

}

int main(int argc, char *argv[]){

	if(argc != 4){
		printf("Usage: ./path_to_charcount ./path_to_text_file ./location_of_huff_encode_out\n");
		return -1;
	}
	
	FILE *finCharCount = fopen(argv[1],"rb");
	if(!finCharCount){
		printf("Charcount file not found.\n");
		return -2;
	}

	FILE *finTextFile = fopen(argv[2],"r");
	if(!finTextFile){
		printf("Text file input not found.\n");
		return -3;
	}

	FILE *outputFile = fopen(argv[3],"wb");
	if(!outputFile){
		printf("Ouput file location not found.\n");
		return -4;
	}

	PriNode *headPriNode = NULL;
	HuffNode *root = NULL;

	buildTree(finCharCount, &headPriNode);

	root = pop(&headPriNode);

	encodeFile(outputFile, finTextFile, root);

	freeTree(root);

	fclose(finCharCount);
	fclose(outputFile);
	fclose(finTextFile);

}
