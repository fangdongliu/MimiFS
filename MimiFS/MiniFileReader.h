#pragma once

//��װ��MiniFile��������
class MiniFileReader
{
public:
	MiniFileReader(MiniFile*);
	~MiniFileReader();
	
	int getBlockMaxReadSize();//��ȡBlock���ɶ��ֽ���
	int readABlock(char*bytes);//��ȡ��һ��block

private:
	BlockHead head;
	int currentBlockId;
	int blockIdTobeRead;
	int remainSize;
};

