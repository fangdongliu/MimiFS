#include "stdafx.h"
#include "MiniFileReader.h"


MiniFileReader::MiniFileReader(MiniFile*file)
{
	head.nextBlockId = file->fileHead.blockId;
}


MiniFileReader::~MiniFileReader()
{
}

int MiniFileReader::getBlockMaxReadSize() {
	return MiniFile::op.getBlockSize() - sizeof(BlockHead);
}

int MiniFileReader::readABlock(char*bytes) {

	if (!head.nextBlockId) {
		return 0;
	}

	MiniFile::op.seekBlock(head.nextBlockId);
	MiniFile::op.read(head);

	fread(bytes, 1, head.size, MiniFile::op.file);

	return head.size;

}