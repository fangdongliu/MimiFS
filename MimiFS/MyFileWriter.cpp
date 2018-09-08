#include "stdafx.h"
#include "MyFileWriter.h"


MyFileWriter::MyFileWriter(std::string&filename)
{
	file = nullptr;
	if (!_access(filename.c_str(), 0)) {
		std::cout << "file:" << filename << " already exists!\n";
	}
	else {
		fopen_s(&file, filename.c_str(), "wb");
	}
}


MyFileWriter::~MyFileWriter()
{
	if (file)
		fclose(file);
}

void MyFileWriter::seekBlock(int blockId) {

	long dest = blockId;
	dest *= blockSize;

	fseek(file, dest, SEEK_SET);
}
