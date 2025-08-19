#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>

#include "file.h"
#include "parse.h"

void print_usage(char * argv[]){
	printf("Usage: %s -n -f <filepath>\n", argv[0]);
	printf("\t -n  - create new database file\n");
	printf("\t -f  - (required) path to database file\n");
	return;
}

int main(int argc, char *argv[]){
	bool newfile = false;
	char *filepath = NULL;
	int opt = 0;

	while ( (opt = getopt(argc, argv, "nf:")) != -1 ){
		switch(opt){
		case 'n':
			newfile = true;
			break;
		case 'f':
			filepath = optarg;
			break;
		case '?':
			printf("Unknown option -%c\n", opt);
			break;
		default:
			return -1;
		}
	}


	if (filepath == NULL){
		printf("Filepath is required\n");
		print_usage(argv);

		return 0;
	}

	printf("Newfile: %d\n",newfile);
	printf("Filepath: %s\n",filepath);
	
	return 0;

}
