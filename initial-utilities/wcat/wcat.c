#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h> 

struct stat filestat;

void throw(){
	printf("wcat: cannot open file\n");
	exit(1);
}

int main(int argc, char *argv[]){

	for(int i=1; i<argc; i++){
		int fd = open(argv[i], 0);
		if(fd == -1 || fstat(fd, &filestat)) throw();

		char *data = mmap(NULL, filestat.st_size, PROT_READ, MAP_SHARED, fd, 0);
		if(data == MAP_FAILED) continue;
		close(fd);

		printf("%s", data);
		if(munmap(data, filestat.st_size)==-1) throw();
	}

	return 0;
}