#pragma once


//封装的MiniFile写操作类
class MiniFileWriter
{
public:
	MiniFileWriter(MiniFile*);//重写一个MiniFile
	~MiniFileWriter();
	
	int getBlockMaxWriteSize();//获取一个Block最大可写字节数
	int writeToBlock(const char*bytes,int bytecount);//将数据写入一个block

private:
	int curBlock;
	BlockHead head;
	MiniFile* file;
};

