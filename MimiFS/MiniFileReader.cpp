#include "stdafx.h"
#include "MiniFileReader.h"


MiniFileReader::MiniFileReader(MiniFile*file)
{
	head.nextBlockId = file->fileHead.blockId;
	remainSize = 0;
}


MiniFileReader::~MiniFileReader()
{
}

int MiniFileReader::getBlockMaxReadSize() {
	return MiniFile::op.getBlockSize();
}

int MiniFileReader::readABlock(char*bytes) {

	if (!head.nextBlockId&&remainSize==0) {
		return 0;
	}

	if (remainSize > 0) {
		int blockSize = getBlockMaxReadSize();

		MiniFile::op.seekBlock(blockIdTobeRead);

		blockIdTobeRead++;

		if (blockSize >= remainSize) {
			fread(bytes, 1, remainSize, MiniFile::op.file);
			int ans = remainSize;
			remainSize = 0;
			return ans;
		}
		else {
			fread(bytes, 1, blockSize, MiniFile::op.file);
			remainSize -= blockSize;
			return blockSize;
		}
	}
	else {
		MiniFile::op.seekBlock(head.nextBlockId);
		blockIdTobeRead = head.nextBlockId + 1;

		MiniFile::op.read(head);

		remainSize = head.size;

		int headBlockSize = getBlockMaxReadSize() - sizeof(BlockHead);

		if (headBlockSize >= head.size) {
			fread(bytes, 1, head.size, MiniFile::op.file);
			remainSize = 0;
			return head.size;
		}
		else {
			fread(bytes, 1, headBlockSize, MiniFile::op.file);
			remainSize -= headBlockSize;
			return headBlockSize;
		}
	}
}