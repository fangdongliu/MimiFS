#include "stdafx.h"
#include "MiniFile.h"
#include "MiniFolder.h"

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

void MiniFile::showMap() {

	BlockHead blockHead;
	blockHead.nextBlockId = fileHead.blockId;

	while (blockHead.nextBlockId) {
		std::cout << blockHead.nextBlockId << ' ';
		op.seekBlock(blockHead.nextBlockId);
		op.read(blockHead);
	}

}

void MiniFile::showAtt() {

	using namespace std;

	tm timeInfo;

	localtime_s(&timeInfo,(const time_t*)&fileHead.createTime);

	timeInfo.tm_year += 1900;

	cout << fileHead.filename << " �ļ���С��" << computeSize() << "bytes" << " �������ڣ�" << timeInfo.tm_year << '-'
		<< timeInfo.tm_mon + 1
		<< '-' << timeInfo.tm_mday << ' ' << timeInfo.tm_hour << ':' << timeInfo.tm_min << ':' << timeInfo.tm_sec << endl;
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

void MiniFile::FileOperator::updateHead() {
	seekBlock(0);
	write(superHead);
	flush();

	using namespace std;
	cout <<"���п����"<< superHead.emptyBlockCount<<endl;
	cout <<"���п�ͷ��Id:" << superHead.firstEmptyBlockId<<"(0/1/2�����)" <<endl;
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

int MiniFile::FileOperator::requestEmptyBlock() {
	//printf("first:%d,hasbefore:%d\n",superHead.firstEmptyBlockId, superHead.emptyBlockCount);


	if (superHead.emptyBlockCount > 0) {
		superHead.emptyBlockCount--;

		

		seekBlock(superHead.firstEmptyBlockId);

		BlockHead blockHead;
		read(blockHead);

		superHead.firstEmptyBlockId = blockHead.nextBlockId;

		reseekCurBlock();

		blockHead.nextBlockId = 0;
		blockHead.size = 0;

		write(blockHead);
		flush();
		return currentBlockId;
	}
	else {
		throw std::exception("space not enough!");
	}
	//printf("first:%d,has:%d\n", superHead.firstEmptyBlockId, superHead.emptyBlockCount);
}

void MiniFile::FileOperator::releaseBlock(int blockId) {	
	//printf("first:%d,hasbefore:%d\n", superHead.firstEmptyBlockId, superHead.emptyBlockCount);
	superHead.emptyBlockCount++;
	BlockHead blockHead;
	seekBlock(blockId);
	read(blockHead);

	while (blockHead.nextBlockId) {
		superHead.emptyBlockCount++;
		seekBlock(blockHead.nextBlockId);
		read(blockHead);
	}

	reseekCurBlock();
	blockHead.nextBlockId = superHead.firstEmptyBlockId;
	write(blockHead);
	flush();
	superHead.firstEmptyBlockId = blockId;
	//printf("first:%d,has:%d\n", superHead.firstEmptyBlockId, superHead.emptyBlockCount);
}

void MiniFile::FileOperator::seekBlock(int blockId) {

	long dest = blockId;
	dest *= superHead.blockSize;

	currentBlockId = blockId;

	fseek(file, dest, SEEK_SET);
}
