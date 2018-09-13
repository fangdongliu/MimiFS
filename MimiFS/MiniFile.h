#pragma once

//Blockͷ�ṹ��
struct BlockHead {
	int size;
	int arraySize;
	int nextBlockId;
};

//�ļ�ͷ�ṹ��
struct MiniFileHead {
	int type;
	int blockId;
	time_t createTime;
	std::string filename;
	int size;
};


//��һ��Blockͷ��
struct SuperHead {
	long fileSize;
	int blockSize;
	int firstEmptyBlockId;
	int emptyBlockCount;
	int arrayCount;
	int lastEmptyBlockId;
};

class MiniFolder;
class ConsoleApp;
class MiniFileWriter;
class MiniFileReader;

//������MiniFS �ļ�
class MiniFile
{
	friend ConsoleApp;
	friend MiniFolder;
	friend MiniFileWriter;
	friend MiniFileReader;
protected:
	//��װ�ļ���д����
	class FileOperator {
		friend ConsoleApp;
		friend MiniFileWriter;
		friend MiniFileReader;
	public:
		FileOperator() {
			file = 0;
			currentBlockId = 0;
		}

		std::string filename;
		SuperHead superHead;
		int currentBlockId;
		FILE * file;

		void updateHead();

		void close();
		void open(std::string& filename);

		void seekBlock(int blockId);
		void reseekCurBlock();

		void getEmptyBlockIds(std::vector<BlockHead>&);

		int getBlockSize() { return superHead.blockSize; }

		void releaseBlock(int blockId);
		int requestEmptyBlock(int count);

		bool ready() { return file != nullptr; }

		void flush() { fseek(file, 0, SEEK_CUR); }

		template<typename T>
		void write(T t) {
			int len = fwrite(&t, 1, sizeof(T), file);

			if (len != sizeof(T))
				throw std::exception("write file failed!");

		}

		template<typename T>
		void read(T& t) {
			int len = fread(&t, 1, sizeof(T), file);

			if (len != sizeof(T))
				throw std::exception("read file failed!");

		}

		template<>
		void read<MiniFileHead>(MiniFileHead& t) {
			read(t.blockId);
			read(t.type);
			read(t.createTime);
			read(t.filename);
			read(t.size);
		}

		template<>
		void write<MiniFileHead>(MiniFileHead t) {
			write(t.blockId);
			write(t.type);
			write(t.createTime);
			write(t.filename);
			write(t.size);
		}

		template<>
		void write<MiniFileHead&>(MiniFileHead& t) {
			write(t.blockId);
			write(t.type);
			write(t.createTime);
			write(t.filename);
			write(t.size);
		}

		template<>
		void write<std::string&>(std::string& t) {
			short len = t.length();
			write(len);

			int len2 = fwrite(t.c_str(), 1, len, file);

			if (len != len2)
				throw std::exception("write file failed!");

		}

		template<>
		void write<std::string>(std::string t) {
			short len = t.length();
			write(len);

			int len2 = fwrite(t.c_str(), 1, len, file);

			if (len != len2)
				throw std::exception("write file failed!");

		}

		template<>
		void read<std::string>(std::string& t) {
			short len;
			read(len);

			char *buf = new char[len + 1];

			int len2 = fread(buf, 1, len, file);

			buf[len] = 0;

			if (len2 != len)
				throw std::exception("read file failed!");

			t = buf;
			delete[]buf;
		}
	};

	//��̬���󣬶�д����ռ�
	static FileOperator op;

	//���ļ�ͷ�����ļ������ڴ�����м���
	static MiniFile* fromFileHead(MiniFileHead&);

	MiniFile();
public:
	~MiniFile();

	//�����ļ���С
	virtual int computeSize() { return fileHead.size; }

	//��ȡ�ļ�����(�������ڣ���С)
	virtual const MiniFileHead& getFileHead();

	//����ɾ������е��ļ�
	virtual int deleteForever();

	//��ȡ�ļ�ռ�ÿ��
	void getMap(std::vector<BlockHead>&);

	//�����ļ�ͷ��С
	int computeHeadSize()const { return 18 + fileHead.filename.length(); }

	//�ж��Ƿ�Ϊ�ļ���
	bool isFolder()const { return fileHead.type == FS_FILETYPE_FOLDER; }


public:
	//getter and setter
	void setParent(MiniFolder*parent) { this->parent = parent; }
	void setFilename(std::string&filename) { fileHead.filename = filename; }
	MiniFolder* getParent()const { return parent; }
	std::string getFilename()const { return fileHead.filename; }

protected:
	MiniFileHead fileHead;
	MiniFolder *parent;
};

