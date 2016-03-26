#include <iostream>

#include "diskobj.hpp"


DiskObj::DiskObj(){ }

DiskObj::~DiskObj() { }

void DiskObj::toString(){
	std::cout <<
		"\n" << 
		"Super block information: \n" <<
		"Block size: " <<  blockSize() << "\n" <<
		"Block count: " << blockCount() << "\n" <<
		"FAT block starts: " << fatStarts() << "\n" <<
		"FAT blocks: " << fatBlocks() << "\n" <<
		"Root directory start: " << rootDirStarts() << "\n" <<
		"Root directory blocks: " << rootDirBlocks() << "\n" <<
		"\n" <<
		"FAT information: " << "\n" <<
		"Free blocks: " << freeBlocks() << "\n" <<
		"Reserved blocks: " << reservedBlocks() << "\n" <<
		"Allocated blocks: " << allocatedBlocks() << 
	std::endl; 
}
