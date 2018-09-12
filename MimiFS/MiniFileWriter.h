#pragma once


//封装的MiniFile写操作类
class MiniFileWriter
{
public:
	MiniFileWriter(MiniFile*);//重写一个MiniFile
	~MiniFileWriter();
	
	int queryCurrentMaxWriteSize();//获取当前Block最大可写字节数
	int writeToBlock(const char*bytes,int bytecount);//将数据写入一个block

private:
	bool isHead;
	int remainSpaceSize;
	int curBlock;
	int curHeadBlock;
	BlockHead head;
	MiniFile* file;
};

