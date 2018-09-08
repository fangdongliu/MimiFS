#pragma once

class MyFileWriter
{
public:
	MyFileWriter(std::string&filename);
	~MyFileWriter();

public:

	void setBlockSize(int size) { blockSize = size; }
	void seekBlock(int blockId);

	bool ready() { return file != nullptr; }

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

