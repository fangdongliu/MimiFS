#pragma once

struct BlockHead {
	int size;
	int nextBlockId;
};

struct MiniFileHead {
	int type;
	int blockId;
	int createTime;
	std::string filename;
	int size;
};

struct SuperHead {
	long fileSize;
	int blockSize;
	int firstEmptyBlockId;
	int emptyBlockCount;
};

class MiniFolder;
class ConsoleApp;
class MiniFileWriter;
class MiniFileReader;

class MiniFile
{
	friend ConsoleApp;
	friend MiniFolder;
	friend MiniFileWriter;
	friend MiniFileReader;
protected:
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

		int getBlockSize() { return superHead.blockSize; }

		void releaseBlock(int blockId);
		int requestEmptyBlock();

		bool ready() { return file != nullptr; }

		void flush() {
			fseek(file, 0, SEEK_CUR);
		}

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

	static FileOperator op;

	static MiniFile* fromFileHead(MiniFileHead&);

	MiniFile();
public:
	~MiniFile();
	virtual void show() {}
	int computeHeadSize() { return 18 + fileHead.filename.length(); }
	void setFilename(std::string&filename) { fileHead.filename = filename; }
	std::string getFilename() { return fileHead.filename; }
	void setParent(MiniFolder*parent) { this->parent = parent; }
	MiniFolder* getParent() { return parent; }

	bool isFolder() { return fileHead.type == FS_FILETYPE_FOLDER; }
	void moveTo(MiniFolder*);
	virtual int deleteForever();


protected:
	MiniFileHead fileHead;
	MiniFolder *parent;
};

