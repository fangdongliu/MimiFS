#include "stdafx.h"
#include "MiniFile.h"
#include "MiniFolder.h"

using namespace std;

MiniFile::FileOperator MiniFile::op;

MiniFile::MiniFile()
{
	time(&fileHead.createTime);
	fileHead.type = FS_FILETYPE_FILE;
	fileHead.size = 0;
}

MiniFile::~MiniFile()
{
}

int MiniFile::deleteForever() {
	op.releaseBlock(fileHead.blockId);
	return 0;
}

void MiniFile::getMap(std::vector<BlockHead>&blockIds) {

	BlockHead blockHead;
	blockHead.nextBlockId = fileHead.blockId;

	while (int curId=blockHead.nextBlockId) {
		op.seekBlock(curId);
		op.read(blockHead);
		blockIds.push_back({ 0 , blockHead.arraySize,curId });
	}

}

const MiniFileHead& MiniFile::getFileHead() {

	return fileHead;

}


MiniFile* MiniFile::fromFileHead(MiniFileHead&head) {

	if (head.type == FS_FILETYPE_FILE) {
		auto file = new MiniFile;
		file->fileHead = head;
		return file;
	}
	else {
		auto folder = new MiniFolder;
		folder->fileHead = head;
		
		return folder;
	}

}

void MiniFile::FileOperator::getEmptyBlockIds(std::vector<BlockHead>&ids) {

	BlockHead head;

	if (superHead.emptyBlockCount == 0)
		return;

	head.nextBlockId = superHead.firstEmptyBlockId;

	while (head.nextBlockId) {
		int id = head.nextBlockId;
		seekBlock(head.nextBlockId);
		read(head);
		ids.push_back({ 0,head.arraySize,id });
	}
	
}

void MiniFile::FileOperator::updateHead() {

	seekBlock(0);
	write(superHead);
	flush();

	cout <<"空闲块个数"<< superHead.emptyBlockCount<<'\n';
	cout <<"空闲块头部Id:" << superHead.firstEmptyBlockId<<"(0/1/2则出错)" <<'\n';
}

void MiniFile::FileOperator::reseekCurBlock() {

	long dest = currentBlockId;
	dest *= superHead.blockSize;

	fseek(file, dest, SEEK_SET);

}

void MiniFile::FileOperator::open(std::string& filename) {

	if (!fopen_s(&file, filename.c_str(), "rb+")) {
		read(superHead);
		currentBlockId = 0;
		this->filename = filename;
	}

}

void MiniFile::FileOperator::close() {

	if (file) {
		seekBlock(0);
		write(superHead);
		flush();
		fclose(file);
		file = 0;
	}
}

int MiniFile::FileOperator::requestEmptyBlock(int count) {

	if (superHead.emptyBlockCount >= count) {
		superHead.emptyBlockCount-=count;

		int ans = superHead.firstEmptyBlockId;

		BlockHead blockHead;//读入BlockHead
		blockHead.arraySize = 0;
		blockHead.nextBlockId = superHead.firstEmptyBlockId;

		do {
			if (blockHead.arraySize) {
				superHead.arrayCount--;
			}
			count -= blockHead.arraySize;
			seekBlock(blockHead.nextBlockId);
			read(blockHead);
		} while (blockHead.arraySize < count);

		if (count == blockHead.arraySize) {
			superHead.firstEmptyBlockId = blockHead.nextBlockId;
			superHead.arrayCount--;
			blockHead.nextBlockId = 0;
			blockHead.size = 0;
			reseekCurBlock();
			write(blockHead);
		}
		else {
			superHead.firstEmptyBlockId = currentBlockId + count;

			BlockHead blockHead2 = blockHead;
			blockHead2.arraySize -= count;

			blockHead.arraySize = count;
			blockHead.nextBlockId = 0;
			blockHead.size = 0;
			reseekCurBlock();
			write(blockHead);

			seekBlock(superHead.firstEmptyBlockId);
			write(blockHead2);
		}

		return ans;
	}
	else {
		throw std::exception("space not enough!");
	}
}

void MiniFile::FileOperator::releaseBlock(int blockId) {	

	BlockHead blockHead;
	blockHead.nextBlockId = blockId;

	int lastCount = superHead.arrayCount;

	do {
		seekBlock(blockHead.nextBlockId);
		read(blockHead);
		superHead.emptyBlockCount += blockHead.arraySize;
		superHead.arrayCount++;
	} while (blockHead.nextBlockId);

	int lastEnd = superHead.lastEmptyBlockId;
	superHead.lastEmptyBlockId = currentBlockId;

	if (lastCount == 1) {
		seekBlock(superHead.firstEmptyBlockId);
		read(blockHead);
		reseekCurBlock();
		blockHead.nextBlockId = blockId;
		write(blockHead);
	}
	else if(lastCount==0){
		superHead.firstEmptyBlockId = blockId;
	}
	else {
		seekBlock(lastEnd);
		read(blockHead);
		reseekCurBlock();
		blockHead.nextBlockId = blockId;
		write(blockHead);
	}
}

void MiniFile::FileOperator::seekBlock(int blockId) {

	long dest = blockId;
	dest *= superHead.blockSize;

	currentBlockId = blockId;

	fseek(file, dest, SEEK_SET);
}
