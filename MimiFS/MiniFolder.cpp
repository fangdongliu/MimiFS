#include "stdafx.h"
#include "MiniFolder.h"


MiniFolder::MiniFolder()
{
	fileHead.type = FS_FILETYPE_FOLDER;
	loaded = false;
}


MiniFolder::~MiniFolder()
{
	clear();
}

int MiniFolder::computeSize() {

	if (!loaded)
		load();

	int size = 0;

	for (auto& i : childs) {

		size += i.second->computeSize();

	}

	return size;
}

const MiniFileHead& MiniFolder::getFileHead() {

	fileHead.size = computeSize();
	return fileHead;

}

void MiniFolder::show() {

	using namespace std;

	for (auto i : childs) {

		if (i.second) {
			if (i.second->isFolder()) {
				cout << "/" << i.first << '\n';
			}
			else {
				cout << i.first << "     " << i.second->fileHead.size << "byte" << '\n';
			}
		}

	}

}

void MiniFolder::findMatchFiles(std::string&pattern, std::vector<MiniFile*>&out) {

	for (auto i : childs) {
		if (i.second) {
			if (Helper::blurMatch(pattern, i.first)) {
				out.push_back(i.second);
			}
		}
	}

}

void MiniFolder::clear() {

	for (auto i : childs) {
		delete i.second;
	}

	childs.clear();

}

std::string MiniFolder::getAbsolutePath() {

	if (parent) {
		return  parent->getAbsolutePath() + "/" + fileHead.filename;
	}

	return  "root:/";

}

MiniFile* MiniFolder::createChildFile(std::string& filename,int blockCount) {

	if (filename.length() > 256) {
		throw FilenameLengthTooLong();
	}

	auto f = new MiniFile();
	f->setParent(this);
	childs[filename] = f;
	f->fileHead.filename = filename;
	f->fileHead.blockId = op.requestEmptyBlock(blockCount);
	return f;
}

MiniFolder* MiniFolder::createChildFolder(std::string& filename) {
	auto f = new MiniFolder();
	f->setParent(this);
	childs[filename] = f;
	f->loaded = false;
	f->fileHead.filename = filename;
	f->fileHead.blockId = op.requestEmptyBlock(1);
	return f;
}

MiniFile*& MiniFolder::atChild(std::string&filename) {

	return childs.at(filename);

}

MiniFile*& MiniFolder::getChild(std::string&fileName) {

	return childs[fileName];

}

void MiniFolder::updateDir() {

	int remainSize = 0;


	BlockHead blockHead;

	op.seekBlock(fileHead.blockId);
	op.read(blockHead);
	op.flush();
	blockHead.size = 0;
	remainSize = op.getBlockSize() - sizeof(BlockHead);
	int curBlockId = fileHead.blockId;

	for (auto& i : childs) {

		if (i.second == nullptr)
			continue;

		auto size = i.second->computeHeadSize();

		if (size > remainSize) {

			if (!blockHead.nextBlockId)
				blockHead.nextBlockId = op.requestEmptyBlock(1);

			op.seekBlock(curBlockId);
			op.write(blockHead);

			op.seekBlock(curBlockId = blockHead.nextBlockId);
			op.read(blockHead);
			op.flush();

			blockHead.size = 0;
			remainSize = op.getBlockSize() - sizeof(BlockHead);
		}

		remainSize -= size;

		op.write<MiniFileHead&>(i.second->fileHead);
		blockHead.size++;

	}

	if (blockHead.nextBlockId)
		op.releaseBlock(blockHead.nextBlockId);

	blockHead.nextBlockId = 0;
	op.seekBlock(curBlockId);
	op.write(blockHead);
	op.flush();

}

int MiniFolder::deleteForever() {

	if (!loaded) {
		load();
	}

	auto&current = ConsoleApp::getInstance()->current;
	if (current->isChildOf(this)) {
		current =parent;
	}

	int count = 0;

	for (auto i : childs) {

		if (i.second) {
			count += i.second->deleteForever();
			delete i.second;
			count++;
		}

	}

	op.releaseBlock(fileHead.blockId);
	return count;
}

bool MiniFolder::isChildOf(MiniFolder*f) {
	auto p = this;
	while (p) {
		if (p == f) {
			return true;
		}
		p = p->parent;
	}
	return false;
}

void MiniFolder::load() {

	BlockHead blockHead;
	blockHead.nextBlockId = fileHead.blockId;

	do {
		op.seekBlock(blockHead.nextBlockId);
		op.read(blockHead);

		for (int i = 0; i < blockHead.size; i++) {
			MiniFileHead filehead;
			op.read(filehead);
			auto file = MiniFile::fromFileHead(filehead);
			file->setParent(this);
			childs[filehead.filename] = file;

		}
	} while (blockHead.nextBlockId);

}

void MiniFolder::printTree(int depth) {

	if (!loaded)
		load();

	std::string str;
	for (int i = 0; i < depth; i++) {
		str += "║  ";
	}
	std::string str2 = str + "╠═";

	for (auto i : childs) {

		if (i.second) {
			printf("%s %s\n", str2.c_str(), i.first.c_str());
			if (i.second->isFolder()) {
				((MiniFolder*)i.second)->printTree(depth + 1);
			}
		}

	}
	printf("%s╙\n", str.c_str());
}

bool MiniFolder::moveFrom(MiniFolder*other, std::string&filename) {

	using namespace std;

	try {

		auto& from = other->atChild(filename);
		auto& to = childs[filename];
		if (to) {
			cout << "目标文件已存在，是否覆盖?[Y/N]:";
			string str;
			getline(cin, str);
			if (!(str.length() == 1 && str[0] == 'y' || str[0] == 'Y')) {
				throw exception("移动失败");
			}
			else {
				to->deleteForever();
			}
		}
		{
			to = from;
			from = nullptr;
			updateDir();
			other->updateDir();
			return true;
		}
	}
	catch (exception&e) {
		throw exception("未找到目标文件");
	}
}