#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "common.h"
#include "parse.h"

int output_file(int fd, struct dbheader_t *dbhdr){
	if (fd < 0){
		printf("Bad FD from user\n");
 		return STATUS_ERROR;
 	}
	
	dbhdr->magic = htonl(dbhdr->magic); // host to network long
	dbhdr->filesize = htonl(dbhdr->filesize);
	dbhdr->count = htons(dbhdr->count); // host to network short
	dbhdr->version = htons(dbhdr->version);
	
	// move cursor to start of file
	lseek(fd, 0, SEEK_SET);
	
	write(fd, dbhdr, sizeof(struct dbheader_t));
	return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut){
	if (fd < 0){
		printf("Bad FD from user\n");
		return STATUS_ERROR;
	}

	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == NULL){
		printf("Malloc failed to create database header\n");
		return STATUS_ERROR;
	}
	
	if ((read(fd, header, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t))){
		perror("read");
		free(header);
		return STATUS_ERROR;
	}
	
	// convert network endianness to host endian
	// ntohs: network to host short
	// ntohl : network to host long (int)
	header->version = ntohs(header->version);
	header->count = ntohs(header->count);
	header->magic = ntohl(header->magic);
	header->filesize = ntohl(header->filesize);
	
	if (header->version != 1){
		printf("Improper header version\n");
		free(header);
		return STATUS_ERROR;
	}
	
	if (header->magic != HEADER_MAGIC){
		printf("Improper header magic\n");
		free(header);
		return STATUS_ERROR;
	}

	struct stat dbstat = {0};
	fstat(fd, &dbstat);
	if (header->filesize != dbstat.st_size){
		printf("Corrupted database file (size error)");
		free(header);
		return STATUS_ERROR;
	}

	*headerOut = header;

	return STATUS_SUCCESS;
}

int create_db_header(int fd, struct dbheader_t **headerOut){
	struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
	if (header == NULL){
		printf("Malloc failed to create db header\n");
		return STATUS_ERROR;
	}

	header->magic = HEADER_MAGIC;
	header->version = 0x1;
	header->count = 0;
	header->filesize = sizeof(struct dbheader_t);
	
	*headerOut = header;
	return STATUS_SUCCESS;
}
