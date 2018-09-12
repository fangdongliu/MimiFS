#pragma once


//��װ��MiniFileд������
class MiniFileWriter
{
public:
	MiniFileWriter(MiniFile*);//��дһ��MiniFile
	~MiniFileWriter();
	
	int queryCurrentMaxWriteSize();//��ȡ��ǰBlock����д�ֽ���
	int writeToBlock(const char*bytes,int bytecount);//������д��һ��block

private:
	bool isHead;
	int remainSpaceSize;
	int curBlock;
	int curHeadBlock;
	BlockHead head;
	MiniFile* file;
};

