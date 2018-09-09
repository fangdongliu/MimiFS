#pragma once

class MiniFileWriter
{
public:
	MiniFileWriter(MiniFile*);//��дһ��MiniFile
	~MiniFileWriter();
	BlockHead head;
	int getBlockMaxWriteSize();
	int writeToBlock(char*bytes,int bytecount);

private:
	int remain;
	int curBlock;
	MiniFile* file;
};

