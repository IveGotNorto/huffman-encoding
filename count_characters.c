#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	// used for checking non ascii values	
	int checkNon = 0;
	int array[128] = {0};
	int store;

	if(argc != 3){
		printf("Usage: ./path_to_text_file ./location_of_output_text \n");
		return -1;
	}
	
	FILE *fin = fopen(argv[1],"r");
	if(!fin){
		printf("Input file not found.\n");
		return -2;
	}

	while(store = fgetc(fin)) {

		if(store == EOF) {
			break;	
		}

		if((store >= 0) || (store < 128)) {
			array[store] = array[store] + 1;
		} else {
			checkNon = 1;
		}
	
	}

	FILE *writ = fopen(argv[2],"wb");
	fwrite(array, sizeof(int), 128, writ);
		
	if(checkNon == 1) {
		printf("\nNon-Ascii characters were found during count.\n");
	}
	
	fclose(fin);
	fclose(writ);

}
