#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define ERR_NO_ARGS 0
#define ERR_BAD_ARGS 1

void throw(int err){
	switch(err){
		case ERR_NO_ARGS:
			puts("wgrep: searchterm [file ...]");
			break;
		case ERR_BAD_ARGS:
			puts("wgrep: cannot open file");
			break;
	}
	exit(1);
}

void grep(FILE *fptr, char *lookup){

	char **lineptr = malloc(sizeof(char**));
	size_t *n = malloc(sizeof(size_t));
	*n = 0;

	while(!feof(fptr)){
		getline(lineptr, n, fptr);
		if(strstr(*lineptr, lookup))
			printf("%s", *lineptr);
	}
}

int main(int argc, char *argv[]){

	if(argc==1) throw(ERR_NO_ARGS);

	else if(argc==2)
		grep(stdin, argv[1]);	

	else{
		for(int i=2; i<argc; i++){
			
			FILE *fptr = fopen(argv[i], "r");
			if(!fptr) throw(ERR_BAD_ARGS);
			
			grep(fptr, argv[1]);
		}
	}

	return 0;
}