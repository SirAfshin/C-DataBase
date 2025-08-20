#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "common.h"
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

	int dbfd = -1; // database file descriptor
	struct dbheader_t *header = NULL;

	
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

	if (newfile){
		dbfd = create_db_file(filepath);
		if (dbfd == STATUS_ERROR){
			printf("Unable to create database file");
			return -1;
		}
		if (create_db_header(dbfd, &header) == STATUS_ERROR){
			printf("Unable to create header meta data\n");
			return -1;
		}
	}
	else {
		dbfd = open_db_file(filepath);
		if (dbfd == STATUS_ERROR){
			printf("Unable to open database file");
			return -1;
		}
		
		if (validate_db_header(dbfd, &header) == STATUS_ERROR){
			printf("Failed to validate database header\n");
			return -1;
		}
	}

	
	printf("Newfile: %d\n",newfile);
	printf("Filepath: %s\n",filepath);

	printf("Header.Magic: %x\n",header->magic);
	printf("Header.Version: %d\n",header->version);
	printf("Header.Count: %d\n",header->count);
	printf("Header.filesize: %d\n",header->filesize);

	output_file(dbfd, header);
	

	printf("\n");
	return 0;

}
