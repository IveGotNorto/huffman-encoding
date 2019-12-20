/*

Displays the contents of an input binary file as readable, space-and-newline-delimited
strings representing each byte.

Example usage:

./display_binary alice.counts 4 | less -N

(If alice.counts is a binary file containing a list of ints, this will print 4 bytes (one int)
per line and route that output to the 'less' text viewer, with the "-N" flag specifying that
the lines should be numbered. Scroll up and down with arrow keys and quit with "q".)

*/


#include <stdio.h>

int main(int argc, char *argv[]){
	
	// print usage and quit if we don't have two command-line arguments
	if(argc != 3){
		printf("Usage: ./display_binary binary_filepath [bytes per line]\n");
		return -1;
	}
	
	// print error and quit if we can't open a binary-read file handler to argv[1]
	FILE *fin = fopen(argv[1],"rb");
	if(!fin){
		printf("Input file not found.\n");
		return -2;
	}
	
	// cast argv[2] to int,
	//    defines how many bytes to print on each line
	int bytesPerLine = atoi(argv[2]);
	
	unsigned char myNextByte;
	int bytesPerLineCounter = 0;
	
	// fread returns "0" when you try to read past end of file,
	// so can put it in the while condition like this to keep
	// reading into myNextByte until we're out of input file
	while(fread(&myNextByte,1,1,fin)){
		
		// initialize bitmask to 10000000 in binary
		unsigned char bitmask = 1 << 7;
		
		// loop covers bitmask values of 10000000, 01000000,
		// ..., 00000010, 00000001
		while(bitmask > 0){
			// bitwise and operator used with bitmask to determine if
			// myNextByte's value at that particular bit position is 1 or 0
			if(bitmask & myNextByte){
				printf("1");
			} else{
				printf("0");
			}
			// bump the "1" in the bitmask 1 position to the right
			bitmask = bitmask >> 1;
		}
		bytesPerLineCounter++;
		
		// print either a space or a \n after the byte, based on argv[2]
		// and the bytesPerLineCounter variable
		if(bytesPerLineCounter==bytesPerLine){
			bytesPerLineCounter = 0;
			printf("\n");
		} else {
			printf(" ");
		}
	}
	
	fclose(fin);
	
	
	// ensures that we end our printout with a \n
	if(bytesPerLineCounter || !argv[2]){
		printf("\n");
	}
	
	return 0;
}