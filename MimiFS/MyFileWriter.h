#pragma once


//封装的File写操作类
class MyFileWriter
{
public:
	MyFileWriter(std::string&filename);
	~MyFileWriter();

public:
	
	void setBlockSize(int size) { blockSize = size; }//设置块大小
	void seekBlock(int blockId);					//块定位

	bool ready() { return file != nullptr; }		//判断就绪

	template<typename T>
	void write(T t) {
		int len = fwrite(&t, 1, sizeof(T), file);

		if (len != sizeof(T))
			throw std::exception("write file failed!");

	}

private:
	int blockSize;
	FILE * file;

};

