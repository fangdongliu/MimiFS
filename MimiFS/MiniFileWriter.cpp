#include "stdafx.h"
#include "MiniFileWriter.h"

MiniFileWriter::MiniFileWriter(MiniFile*file)
{
	head.size = 0;
	curBlock = 0;
	curHeadBlock = 0;
	remainSpaceSize = 0;
	head.nextBlockId = file->fileHead.blockId;
	this->file = file;
	file->fileHead.size = 0;
}


MiniFileWriter::~MiniFileWriter()
{
	if (curHeadBlock) {

		if (head.nextBlockId) {
			MiniFile::op.releaseBlock(head.nextBlockId);
			head.nextBlockId = 0;
		}

		MiniFile::op.seekBlock(curHeadBlock);
		MiniFile::op.write(head);
	}

}

int MiniFileWriter::queryCurrentMaxWriteSize() {

	if (remainSpaceSize == 0) {

		if (!head.nextBlockId) {
			head.nextBlockId = MiniFile::op.requestEmptyBlock(1);
			MiniFile::op.seekBlock(curHeadBlock);
			MiniFile::op.write(head);
		}

		curHeadBlock = head.nextBlockId;
		curBlock = curHeadBlock;
		MiniFile::op.seekBlock(head.nextBlockId);
		MiniFile::op.read(head);
		isHead = true;
		remainSpaceSize = head.arraySize - 1;

		return MiniFile::op.getBlockSize() - sizeof(BlockHead);
	}
	else {
		isHead = false;
		remainSpaceSize--;
		curBlock++;
		return MiniFile::op.getBlockSize();
	}
}


int MiniFileWriter::writeToBlock(const char*bytes, int bytecount) {

	MiniFile::op.seekBlock(curBlock);
	if (isHead) {
		MiniFile::op.write(head);
	}
	fwrite(bytes, 1, bytecount, MiniFile::op.file);
	head.size += bytecount;
	
	file->fileHead.size += bytecount;

	return bytecount;

}