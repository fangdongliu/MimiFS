#pragma once

//封装的MiniFile读操作类
class MiniFileReader
{
public:
	MiniFileReader(MiniFile*);
	~MiniFileReader();
	
	int getBlockMaxReadSize();//获取Block最大可读字节数
	int readABlock(char*bytes);//读取下一个block

private:
	BlockHead head;
	int currentBlockId;
	int blockIdTobeRead;
	int remainSize;
};

