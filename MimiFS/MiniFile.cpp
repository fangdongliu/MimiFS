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

void MiniFile::showMap(std::vector<int>&blockIds) {

	BlockHead blockHead;
	blockHead.nextBlockId = fileHead.blockId;

	while (blockHead.nextBlockId) {
		blockIds.push_back(blockHead.nextBlockId);
		op.seekBlock(blockHead.nextBlockId);
		op.read(blockHead);
	}

}

void MiniFile::showAtt() {

	tm timeInfo;

	localtime_s(&timeInfo,(const time_t*)&fileHead.createTime);

	timeInfo.tm_year += 1900;

	cout << fileHead.filename << " 文件大小：" << computeSize() << "bytes 创建日期：" 
		<< timeInfo.tm_year << '-'<< timeInfo.tm_mon + 1<< '-' << timeInfo.tm_mday 
		<< ' ' << timeInfo.tm_hour << ':' << timeInfo.tm_min << ':' << timeInfo.tm_sec << '\n';
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

void MiniFile::FileOperator::getEmptyBlockIds(std::vector<std::pair<int,int>>&ids, int maxCount) {

//	BlockHead head;
//
//	head.nextBlockId = superHead.firstEmptyBlockId;
//
//	int n = max(superHead.emptyBlockCount, maxCount);
////	int last = ids;
//	while (n--) {
//		seekBlock(head.nextBlockId);
//		ids.push_back(head.nextBlockId);
//		read(head);
//	}
	
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

	if (superHead.emptyBlockCount > count) {
		superHead.emptyBlockCount-=count;

		int ans = superHead.firstEmptyBlockId;

		BlockHead blockHead;//读入BlockHead
		blockHead.arraySize = 0;
		blockHead.nextBlockId = superHead.firstEmptyBlockId;


		do {
			count -= blockHead.arraySize;
			seekBlock(blockHead.nextBlockId);
			read(blockHead);
		} while (blockHead.arraySize < count);

		if (count == blockHead.arraySize) {
			superHead.firstEmptyBlockId = blockHead.nextBlockId;
			blockHead.nextBlockId = 0;
			blockHead.size = 0;
			reseekCurBlock();
			write(blockHead);
			flush();
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
			flush();
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

	do {
		seekBlock(blockHead.nextBlockId);
		read(blockHead);
		superHead.emptyBlockCount += blockHead.arraySize;
	} while (blockHead.nextBlockId);

	reseekCurBlock();
	blockHead.nextBlockId = superHead.firstEmptyBlockId;
	write(blockHead);
	flush();
	superHead.firstEmptyBlockId = blockId;
}

void MiniFile::FileOperator::seekBlock(int blockId) {

	long dest = blockId;
	dest *= superHead.blockSize;

	currentBlockId = blockId;

	fseek(file, dest, SEEK_SET);
}
