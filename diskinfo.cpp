#include <iostream>
#include <cstdlib>
#include <stdio.h>

#include "diskinfo.hpp"
#include "diskobj.hpp"

#define IDENT_OFFSET		0
#define BLOCKSIZE_OFFSET	8
#define IDENT_SIZE			8

DiskObj disk;


void getName(FILE *fp, char *name)
{
	fseek(fp,SEEK_SET,IDENT_OFFSET);
	fread(name,1,IDENT_SIZE,fp);
}

int main(int argc, char const *argv[])
{
	FILE *fp;
	char *name = (char*)malloc(sizeof(char)*IDENT_SIZE);
	int size;	
	if ((fp=fopen(argv[1],"r"))){
		getName(fp,name);
		std::cout << "name is " << name << std::endl;
	} else
		printf("Fail to open the image file.\n");



	disk.toString();
	return 0;
}
