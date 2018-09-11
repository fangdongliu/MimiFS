#pragma once


//��װ��Fileд������
class MyFileWriter
{
public:
	MyFileWriter(std::string&filename);
	~MyFileWriter();

public:
	
	void setBlockSize(int size) { blockSize = size; }//���ÿ��С
	void seekBlock(int blockId);					//�鶨λ

	bool ready() { return file != nullptr; }		//�жϾ���

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

