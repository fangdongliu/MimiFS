#pragma once


//��װ��MiniFileд������
class MiniFileWriter
{
public:
	MiniFileWriter(MiniFile*);//��дһ��MiniFile
	~MiniFileWriter();
	
	int getBlockMaxWriteSize();//��ȡһ��Block����д�ֽ���
	int writeToBlock(const char*bytes,int bytecount);//������д��һ��block

private:
	int curBlock;
	BlockHead head;
	MiniFile* file;
};

