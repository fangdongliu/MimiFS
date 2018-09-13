#pragma once

//Block头结构体
struct BlockHead {
	int size;
	int arraySize;
	int nextBlockId;
};

//文件头结构体
struct MiniFileHead {
	int type;
	int blockId;
	time_t createTime;
	std::string filename;
	int size;
};


//第一个Block头部
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

//基本的MiniFS 文件
class MiniFile
{
	friend ConsoleApp;
	friend MiniFolder;
	friend MiniFileWriter;
	friend MiniFileReader;
protected:
	//封装文件读写操作
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

	//静态对象，读写载入空间
	static FileOperator op;

	//从文件头创建文件，用于从外存中加载
	static MiniFile* fromFileHead(MiniFileHead&);

	MiniFile();
public:
	~MiniFile();

	//计算文件大小
	virtual int computeSize() { return fileHead.size; }

	//获取文件属性(创建日期，大小)
	virtual const MiniFileHead& getFileHead();

	//永久删除外存中的文件
	virtual int deleteForever();

	//获取文件占用块号
	void getMap(std::vector<BlockHead>&);

	//计算文件头大小
	int computeHeadSize()const { return 18 + fileHead.filename.length(); }

	//判断是否为文件夹
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

