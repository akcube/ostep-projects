#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h> 

#define BUFF_SIZE 1024

struct stat filestat;

void throw(){
	puts("wzip: file1 [file2 ...]");
	exit(1);
}

struct encoding{
	int sz;
	char ch;
} __attribute__((__packed__));

typedef struct encoding encoding;

void map_file_to_mem(char *filename, char **data, int *file_size){
	int fd = open(filename, 0);
	if(fd == -1 || fstat(fd, &filestat)) throw();

	*file_size = filestat.st_size;
	*data = mmap(NULL, *file_size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
}

int main(int argc, char *argv[]){

	if(argc==1) throw();

	//Read file to memory
	int j=1;
	int n=0;
	char *data = NULL;
	map_file_to_mem(argv[j], &data, &n);

	//Compress file
	char c = data[0];
	encoding *buffer = malloc(sizeof(encoding)*BUFF_SIZE);
	int bufptr = 0;
	char *v_file = data;

	for(int i=0, prev=0; i<n; i++){
		
		while(v_file[i]==c) {
			if(i==n-1){
				j++, i++;
				if(j==argc) break;

				int fs;
				for(;j<argc; j++){
					map_file_to_mem(argv[j], &data, &fs);
					if(data==MAP_FAILED) continue;
					break;
				}
				n += fs;
				v_file -= fs;
			}
			else i++;
		}

		buffer[bufptr].sz = i-prev;
		buffer[bufptr++].ch = v_file[i-1];
		c = v_file[i];
		prev = i--;

		if(bufptr==BUFF_SIZE){
			bufptr = 0;
			fwrite(buffer, sizeof(encoding), BUFF_SIZE, stdout);
		}
	}	
	if(bufptr) fwrite(buffer, sizeof(encoding), bufptr, stdout);

	//Free file in memory
	if(munmap(data, n)==-1) throw();

	return 0;
}