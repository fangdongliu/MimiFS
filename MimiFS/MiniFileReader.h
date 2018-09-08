#pragma once
class MiniFileReader
{
public:
	MiniFileReader(MiniFile*);
	~MiniFileReader();
	BlockHead head;
	int getBlockMaxReadSize();
	int readABlock(char*bytes);
};

