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
	puts("wunzip: file1 [file2 ...]");
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
	for(int j=1; j<argc; j++){
		int fs=0;
		encoding *data = NULL;
		map_file_to_mem(argv[j], (char**) &data, &fs);

		int n = fs/(sizeof(encoding));
		for(int i=0; i<n; i++){
			int reps = data[i].sz;
			char c = data[i].ch;

			for(int k=0; k<reps; k++) printf("%c", c);
		}

		//Free file in memory
		if(munmap(data, n)==-1) throw();
	}

	return 0;
}