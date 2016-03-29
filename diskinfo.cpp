#include <iostream>
#include <cstdlib>
#include <cstdio>

#include "diskinfo.hpp"
#include "diskobj.hpp"

#define IDENT_OFFSET		0
#define BLOCKSIZE_OFFSET	8 
#define IDENT_SIZE			8

DiskObj disk;

int getBlockSize(FILE *diskIn)
{
	unsigned char high;
	unsigned char low;
	// std::cout << sizeof(high) << std::endl;
	int size;
	fseek(diskIn,SEEK_SET,BLOCKSIZE_OFFSET);
	// fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
	fread(&high,1,1,diskIn);
	fread(&low,1,1,diskIn);
	size = ((high)<<8) + low;
	return size;
}

int getBlockDeets(FILE *diskIn, int offset)
{
	unsigned char highFirst, lowFirst, highSecond, lowSecond;
	int count;
	fseek(diskIn, SEEK_SET, offset);
	fread(&highFirst, 1, 1, diskIn);
	fread(&lowFirst, 1, 1, diskIn);
	fread(&highSecond, 1, 1, diskIn);
	fread(&lowSecond, 1, 1, diskIn);
	count = ((highFirst)<<8) + lowFirst + ((highSecond)<<8) + lowSecond;
	return count;
}

void getFatDeets(FILE *diskIn)
{
	int keepOffset;
	unsigned char highFirst, lowFirst, highSecond, lowSecond;
	int blockEntry;
	int numAvail = 0;
	int numInUse = 0;
	int numAllocated = 0;
	int wtfOkay = 28;
	for (int offset = 30; 
		offset < (disk.fatBlocks() * disk.blockSize()) + wtfOkay; 
		offset += 4)
	{
		fseek(diskIn, SEEK_SET, offset);
		fread(&highFirst, 1, 1, diskIn);
		fread(&lowFirst, 1, 1, diskIn);
		fread(&highSecond, 1, 1, diskIn);
		fread(&lowSecond, 1, 1, diskIn);
		blockEntry = ((highFirst)<<8) + lowFirst + ((highSecond)<<8) + lowSecond;
		if (blockEntry == 0)
			numAvail ++;
		else if (blockEntry == 1)
			numInUse ++;
		else 
			numAllocated ++;
		keepOffset = offset;
	}
	disk.setFreeBlocks(numAvail);
	disk.setReservedBlocks(numInUse);
	disk.setAllocatedBlocks(numAllocated);
	// std::cout << "offset now is " << keepOffset << std::endl;
}

void getName(FILE *diskIn, char *name)
{
	fseek(diskIn,SEEK_SET,IDENT_OFFSET);
	fread(name,1,IDENT_SIZE,diskIn); // This is the crucial line
}

int main(int argc, char const *argv[])
{
	FILE *diskIn;
	char *name = (char*)malloc(sizeof(char)*IDENT_SIZE);
	// diskIn is the name of the disk image file
	if ((diskIn=fopen(argv[1],"r"))){
		getName(diskIn,name);
		// std::cout << "name is " << name << std::endl;
		disk.setBlockSize(getBlockSize(diskIn));
		disk.setBlockCount(getBlockDeets(diskIn, BLOCKCOUNT_OFFSET));
		disk.setFatStarts(getBlockDeets(diskIn, FATSTART_OFFSET));
		disk.setFatBlocks(getBlockDeets(diskIn, FATBLOCKS_OFFSET));
		disk.setRootDirStarts(getBlockDeets(diskIn, ROOTDIRSTART_OFFSET));
		disk.setRootDirBlocks(getBlockDeets(diskIn, ROOTDIRBLOCKS_OFFSET));
		getFatDeets(diskIn);
	} else
		printf("Fail to open the image file.\n");

	// Print all
	disk.toString();
	return 0;
}
