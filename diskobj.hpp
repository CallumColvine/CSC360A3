class DiskObj
{
private:
	// Super block info
	int _blockSize;
	int _blockCount;
	int _fatStarts;
	int _fatBlocks;
	int _rootDirStarts;
	int _rootDirBlocks;
	// Fat block info
	int _freeBlocks;
	int _reservedBlocks;
	int _allocatedBlocks;
public:
	DiskObj();
	~DiskObj();
	void toString();
	// Super block getters
	int blockSize() { return _blockSize; }
	int blockCount() { return _blockCount; }
	int fatStarts() { return _fatStarts; }
	int fatBlocks() { return _fatBlocks; }
	int rootDirStarts() { return _rootDirStarts; }
	int rootDirBlocks() { return _rootDirBlocks; }
	// Fat getters
	int freeBlocks() { return _freeBlocks; }
	int reservedBlocks() { return _reservedBlocks; }
	int allocatedBlocks() { return _allocatedBlocks; }
	// Super block setters
	void setBlockSize(int inSize) { _blockSize = inSize; }
	void setBlockCount(int inCount) { _blockCount = inCount; }
	void setFatStarts(int inStarts) { _fatStarts = inStarts; }
	void setFatBlocks(int inBlocks) { _fatBlocks = inBlocks; }
	void setRootDirStarts(int inStarts) { _rootDirStarts = inStarts; }
	void setRootDirBlocks(int inBlocks) { _rootDirBlocks = inBlocks; }
	// Fat setters
	void setFreeBlocks(int inFree) { _freeBlocks = inFree; }
	void setReservedBlocks(int inReserved) { _reservedBlocks = inReserved; }
	void setAllocatedBlocks(int inAlloc) { _allocatedBlocks = inAlloc; }


};