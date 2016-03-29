// // Root dir is right after le FAT
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "diskinfo.hpp"

#define IDENT_OFFSET		0
#define BLOCKSIZE_OFFSET	8 
#define IDENT_SIZE			8


int blockSize;

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
	count = ((highFirst)<<24) + (lowFirst<<16) + ((highSecond)<<8) + lowSecond;
	return count;
}

void getName(FILE *diskIn, char *name)
{
	fseek(diskIn,SEEK_SET,IDENT_OFFSET);
	fread(name,1,IDENT_SIZE,diskIn); // This is the crucial line
}

int getBit(unsigned int byte, int whichBit)
{
    if (whichBit > 0 && whichBit <= 8)
        return (byte & (1<<(whichBit-1)));
    else
        return 0;
}

int findSize(FILE* diskIn, int offset){
	// 4 long...
	unsigned char highFirst, lowFirst, highSecond, lowSecond;
	fseek(diskIn, (offset+9), SEEK_SET);
	fread(&highFirst, 1, 1, diskIn);
	fread(&lowFirst, 1, 1, diskIn);
	fread(&highSecond, 1, 1, diskIn);
	fread(&lowSecond, 1, 1, diskIn);
	int size = ((highFirst)<<24) + (lowFirst << 16) + ((highSecond)<<8) + lowSecond;
	return size;
}

void findTime(FILE* diskIn, int offset){
	// 7 bytes...
	unsigned char h1, h2, h3, l1, l2, l3, l4;
	fseek(diskIn, (offset+13), SEEK_SET);
	fread(&h1, 1, 1, diskIn);
	fread(&l1, 1, 1, diskIn);
	fread(&h2, 1, 1, diskIn);
	fread(&l2, 1, 1, diskIn);
	fread(&h3, 1, 1, diskIn);
	fread(&l3, 1, 1, diskIn);
	fread(&l4, 1, 1, diskIn);
	int year = (h1 << 8) + l1;
	int month = h2;
	int day = l2;
	int hour = h3;
	int minute = l3;
	int second = l4;
	std::cout << year << "/" << month << "/" << day << " " << hour << ":" << minute << ":" << second << std::endl; 
	// toRet = (year+"/"+month+"/"+day);
	// +'/'+' '+hour+':'+minute+':'+second 
	// std::cout << "day is " << day << std::endl;
}

void copyFromRoot(FILE *diskIn, int curFileSize, int offset, char* name){
	FILE *curDirOut;
	std::cout << "Found the file!" << std::endl;
	curDirOut = fopen (name, "w");
	int end = (offset + curFileSize);
	unsigned char toWrite;
	std::cout << "Offset is: " << offset << "\n" <<
		"fileSize (bytes) is " << curFileSize << "\n" << 
		"end if " << end << std::endl;
	fseek(diskIn, offset, SEEK_SET);
	for (offset; offset < end; offset++){
		fread(&toWrite, 1, 1, diskIn);
		fwrite(&toWrite, 1, 1, curDirOut);
		std::cout << "Offset is " << offset << std::endl;
	}

}

bool findIsName(FILE *diskIn, int offset, char* name){
	// 31 long...
	char h1[31];
	fseek(diskIn, (offset+27), SEEK_SET);
	fread(h1, 31, 1, diskIn);
	if (strcmp(h1, name) == 0)
		return true;	
	else
		return false;
}

int findStart(FILE* diskIn, int offset){
	unsigned char h1, l1, h2, l2;
	int start;
	fseek(diskIn, (offset+1), SEEK_SET);
	fread(&h1, 1, 1, diskIn);
	fread(&l1, 1, 1, diskIn);
	fread(&h2, 1, 1, diskIn);
	fread(&l2, 1, 1, diskIn);
	start = (h1 << 24) + (l1 << 16) + (h2 << 8) + l2;
	return start;
}

void findFileInRoot(FILE *diskIn, 
					int offset, 
					int endRootOffset, 
					char* fileName)
{
	unsigned char status;
	std::cout << "offset is: " << offset << "\n" <<
		"endRootOffset is: " << endRootOffset << std::endl;
	for (offset; offset < endRootOffset; offset++)
	{
		fseek(diskIn, offset, SEEK_SET);
		fread(&status, 1, 1, diskIn);
		// if (getBit((int)status, 1) == 3){
		if ((int)status == 3){
			int size = findSize(diskIn, offset);
			int startPos = findStart(diskIn, offset);
			// std::cout << "F" << "   " << size << "    ";
			bool isName = findIsName(diskIn, offset, fileName);			
			if (isName){
				std::cout << "start pos is " << startPos << 
				" block size is " << blockSize << std::endl;
				int fileStartOffset = (startPos*blockSize);
				copyFromRoot(diskIn, size, fileStartOffset, fileName);
			}
			offset += 63;
		}
	}	
}

int main(int argc, char const *argv[])
{
	FILE *diskIn;
	int fatStarts, fatBlocks, rootBlocks;
	char *name = (char*)malloc(sizeof(char)*IDENT_SIZE);
	// diskIn is the name of the disk image file
	if ((diskIn=fopen(argv[1],"r"))){
		getName(diskIn,name);
		// std::cout << "name is " << name << std::endl;
		blockSize = getBlockSize(diskIn);
		getBlockDeets(diskIn, BLOCKCOUNT_OFFSET);
		fatStarts = getBlockDeets(diskIn, FATSTART_OFFSET);
		fatBlocks = getBlockDeets(diskIn, FATBLOCKS_OFFSET);
		getBlockDeets(diskIn, ROOTDIRSTART_OFFSET);
		rootBlocks = getBlockDeets(diskIn, ROOTDIRBLOCKS_OFFSET);
		// getFatDeets(diskIn);
	} else
		printf("Fail to open the image file.\n");
	// std::cout << "Starts:
	int rootOffset = (fatBlocks) * blockSize;
	// listRoot(diskIn, rootOffset, rootOffset + (rootBlocks * blockSize));
	findFileInRoot(diskIn, rootOffset, rootOffset + (rootBlocks * blockSize), (char*)argv[2]);
	return 0;
}

