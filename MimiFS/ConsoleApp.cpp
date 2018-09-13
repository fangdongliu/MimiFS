#include "stdafx.h"
#include "ConsoleApp.h"
#include "CommandHandler.h"

using namespace std;

ConsoleApp* ConsoleApp::instance = nullptr;

ConsoleApp * ConsoleApp::getInstance() {
	if (!instance) {
		instance = new ConsoleApp;
	}
	return instance;
}

ConsoleApp::ConsoleApp() {

	helpList["close"] = { "关闭MiniFS系统","close" };
	helpList["create"] = { "创建一个MiniFS系统","create \"filename\" [fileSize] [blockSize]","filename : 要创建系统的文件名\nfileSize : 系统的文件大小\nblockSize : 系统每个块的大小" };
	helpList["mount"] = { "装载一个MiniFS系统","mount \"filename\"","filename : 要装载系统的文件名" };
	helpList["help"] = { "显示帮助","help [\"cmd\"]","cmd : 可选参数，若指定则查看其详细情况，否则列出所有命令" };
	helpList["fmt"] = { "格式化MiniFS系统","fmt [fileSize] [blockSize]","fileSize : 系统的文件大小\nblockSize : 系统每个块的大小" };
	helpList["info"] = { "显示MiniFS系统状态信息","info","" };

}

int ConsoleApp::getBlockSize() {

	return MiniFile::op.getBlockSize();
}

bool ConsoleApp::ready() {
	return MiniFile::op.ready();
}

void ConsoleApp::printPrefix() {

	if (MiniFile::op.ready())
		std::cout << current->getAbsolutePath() + " > ";
	else
		std::cout << "MimiFS > ";

}

void ConsoleApp::handleCommand(Lexer&lexer) {

	try {
		if (lexer.getName() == "mount")
			mountMiniFS(lexer);
		else if (lexer.getName() == "create")
			createMiniFS(lexer);
		else if (lexer.getName() == "help")
			showHelp(lexer);
		else if (!ready()) //判断系统是否加载
			cout << "use \"mount\" to load a miniFS-space OR use \"create\" to create a miniFS-space\n";
		else if (lexer.getName() == "fmt")
			formatMiniFS(lexer);
		else if (lexer.getName() == "close")
			closeMiniFS(lexer);
		else if (lexer.getName() == "info")
			showFSInfo(lexer);
		else if (lexer.getName() == "opt")
			optFS(lexer);
		else {
			CommandHandler *dealer = nullptr;

			dealer = handler.at(lexer.getName());
			dealer->onHandleCommand(lexer);

			if (ready())
				MiniFile::op.updateHead();
		}
	}
	catch (const out_of_range& e)
	{
		cout << "wrong command\n";
	}
	catch (const CommandFormatError&e) {
		cout << "command format error:\n";
		cout << "the right usage : " << helpList[lexer.getName()].format << '\n';
	}
	catch (const InvalidFilename&e) {
		cout << "不合法的文件名\n";
	}
	catch (const FilenameLengthTooLong&e) {
		cout << "文件名过长\n";
	}
	catch (const std::exception& e) {
		cout << e.what() << '\n';
	}
}

//bool ConsoleApp::doOpt(std::vector<int>emptys, MiniFile*f) {
//
//	if (f->isFolder()) {
//
//		bool needupdate = false;
//
//		for (auto i : ((MiniFolder*)f)->childs) {
//			if (i.second) {
//				needupdate |= doOpt(emptys, i.second);
//			}
//		}
//
//		if (needupdate) {
//			((MiniFolder*)f)->updateDir();
//		}
//
//		return false;
//	}
//	else {
//
//		std::vector<int>maps;
//		f->showMap(maps);
//
//		int last = *maps.begin() - 1;
//		bool ordered = true;
//		for (auto i : maps) {
//			if (i != last + 1) {
//				ordered = false;
//				break;
//			}
//			last++;
//		}
//
//		if (ordered) {
//			return false;
//		}
//
//
//
//		for (int i = 0; i < maps.end(); i++) {
//
//		}
//
//	}
//
//
//
//}

void ConsoleApp::optFS(Lexer&param) {

	param >= Lexer::end;

	vector<BlockHead>emptys;

	MiniFile::op.getEmptyBlockIds(emptys);

	if (int n = emptys.size()) {
		std::sort(emptys.begin(), emptys.end(), [](BlockHead&a, BlockHead&b)->bool {
			return a.nextBlockId < b.nextBlockId;
		});

		BlockHead* blocks = new BlockHead[n];
		auto iter = emptys.begin();
		blocks[0] = *iter;
		int cur = 0;
		while (++iter != emptys.end()) {

			if (blocks[cur].nextBlockId + blocks[cur].arraySize == iter->nextBlockId) {
				blocks[cur].arraySize += iter->arraySize;
			}
			else {
				blocks[++cur] = *iter;
			}

		}

		sort(blocks, blocks + cur + 1, [](BlockHead&a, BlockHead&b)->bool {
			if (a.arraySize == 1) {
				if (b.arraySize != 1) {
					return false;
				}
			}
			else if (b.arraySize == 1) {
				return true;
			}
			return a.nextBlockId < b.nextBlockId;
		});

		auto & op = MiniFile::op;

		op.superHead.firstEmptyBlockId = blocks[0].nextBlockId;
		op.superHead.lastEmptyBlockId = blocks[cur].nextBlockId;

		for (int i = 0; i < cur; i++) {			
			op.seekBlock(blocks[i].nextBlockId);
			blocks[i].nextBlockId = blocks[i + 1].nextBlockId;
			op.write(blocks[i]);
		}

		op.seekBlock(blocks[cur].nextBlockId);
		blocks[cur].nextBlockId = 0;
		op.write(blocks[cur]);

		delete[]blocks;
	}
}


void ConsoleApp::showFSInfo(Lexer&param) {

	param >= Lexer::end;

	cout << "系统名 : " + MiniFile::op.filename << '\n';
	cout << "空闲块个数" << MiniFile::op.superHead.emptyBlockCount << '\n';
	cout << "空闲块头部Id:" << MiniFile::op.superHead.firstEmptyBlockId << "(0/1/2则出错)" << '\n';

	vector<BlockHead>emptys;

	MiniFile::op.getEmptyBlockIds(emptys);

	for (auto i : emptys) {

		if (i.arraySize == 1) {
			printf("%d ", i.nextBlockId);
		}
		else {
			printf("(%d-%d) ", i.nextBlockId, i.nextBlockId - 1 + i.arraySize);
		}

	}
	printf("\n");
}

void ConsoleApp::showHelp(Lexer& param) {

	string cmd;

	param > cmd >= Lexer::end;

	if (cmd.length()) {
		try
		{
			auto i = helpList.at(cmd);
			cout << i.title << '\n';
			cout << i.format << '\n';
			cout << i.detail << '\n';
		}
		catch (const std::exception&)
		{
			cout << "目标命令不存在" << '\n';
		}
	}
	else {
		string out;

		std::vector<std::pair<std::string, HelpItem>>strs(helpList.begin(), helpList.end());
		std::sort(strs.begin(), strs.end(), [](std::pair<std::string, HelpItem>&a, std::pair<std::string, HelpItem>&b)->bool {
			return a.first < b.first;
		});

		char buf[100];

		for (auto i : strs) {
			sprintf_s(buf, "%- 8s : %- 24s %- 40s\n", i.first.c_str(), i.second.title.c_str(), i.second.format.c_str());
			printf(buf);
		}
	}

}

void ConsoleApp::closeMiniFS(Lexer&param) {

	param >= Lexer::end;

	MiniFile::op.close();
	root.clear();
	current = &root;
	std::cout << "miniFS 系统( " + MiniFile::op.filename + " )关闭" << '\n';

}

void ConsoleApp::formatMiniFS(Lexer& param) {

	SuperHead head;
	head.lastEmptyBlockId = 0;
	head.fileSize = 1 << 30;
	head.blockSize = 1 << 12;
	head.arrayCount = 1;

	param > head.fileSize > head.blockSize >= Lexer::end;

	head.firstEmptyBlockId = 3;
	head.emptyBlockCount = head.fileSize / head.blockSize - 2;

	auto &f = MiniFile::op;

	root.clear();//重置内存根目录与当前目录
	current = &root;

	f.superHead = head;//写入文件头
	f.seekBlock(0);
	f.write(head);

	BlockHead blockHead;
	blockHead.arraySize = 1;
	blockHead.nextBlockId = 0;
	blockHead.size = 0;

	f.seekBlock(1);		//覆盖root文件夹
	f.write(blockHead);

	f.seekBlock(2);		//覆盖回收站
	f.write(blockHead);

	blockHead.arraySize = head.emptyBlockCount;
	//建立空闲链
	f.seekBlock(3);
	f.write(blockHead);

	//文件尾部标记
	f.seekBlock(head.emptyBlockCount + 3);
	f.write<char>(0);

	std::cout << "miniFS 系统( " + f.filename + " )格式化完成" << '\n';
}

void ConsoleApp::mountMiniFS(Lexer& param) {

	string filename;

	param >= filename >= Lexer::end;

	//关闭已装载系统
	if (ready()) {
		string close = "";
		Lexer closeParam(close);
		closeMiniFS(closeParam);
	}

	//补全.fs后缀
	if (filename.length() < 3)
		filename += ".fs";
	else if (filename.substr(filename.length() - 3) != ".fs")
		filename += ".fs";

	MiniFile::op.open(filename);


	if (!ready()) {
		std::cout << "文件:" << filename << " 不存在或被占用" << '\n';
		return;
	}

	root.load();//加载root目录
	current = &root;//重置当前文件夹
	std::cout << "miniFS 系统( " + filename + " )加载完毕" << '\n';

}

void ConsoleApp::createMiniFS(Lexer& param) {

	std::string filename;
	SuperHead head;
	head.arrayCount = 1;
	head.lastEmptyBlockId = 0;
	head.fileSize = 1 << 30;
	head.blockSize = 1 << 12;

	param >= filename > head.fileSize > head.blockSize >= Lexer::end;

	//补全.fs后缀
	if (filename.length() < 3)
		filename += ".fs";
	else if (filename.substr(filename.length() - 3) != ".fs")
		filename += ".fs";
	{
		MyFileWriter writer(filename);
		if (!writer.ready()) {
			return;
		}
		head.firstEmptyBlockId = 3;
		head.emptyBlockCount = head.fileSize / head.blockSize - 2;


		writer.write(head);//写入文件头
		writer.setBlockSize(head.blockSize);

		BlockHead blockHead;
		blockHead.arraySize = 1;
		blockHead.nextBlockId = 0;
		blockHead.size = 0;

		writer.seekBlock(1);//写入root目录
		writer.write(blockHead);

		writer.seekBlock(2);//写入回收站
		writer.write(blockHead);

		blockHead.arraySize = head.emptyBlockCount;
		//建立空闲链
		writer.seekBlock(3);
		writer.write(blockHead);

		//文件尾部标记
		writer.seekBlock(head.emptyBlockCount + 3);
		writer.write<char>(0);
		//	writer.seekBlock(0);
	}
	std::cout << "miniFS 系统( " + filename + " )创建完成" << '\n';
}

MiniFile* ConsoleApp::getFileByPath(std::vector<std::string>&pathList) {

	string filename = pathList.back();
	pathList.pop_back();

	if (auto folder = getFolderByPath(pathList, false)) {

		if (filename == ".") {
			return folder;
		}
		else if (filename == "..") {
			return folder->parent;
		}
		else {
			try {
				return folder->atChild(filename);
			}
			catch(exception&e){
				throw exception("文件不存在");
			}
		}
	}
	else {
		throw PathNotExist();
	}

}

MiniFolder* ConsoleApp::getFolderByPath(std::vector<std::string>&pathlist, bool queryCreateNew) {

	MiniFolder* dest = current;

	auto it = pathlist.begin();

	if (it == pathlist.end())
		return dest;

	if (*it == "root:") {//绝对路径
		dest = &root;
		it++;
	}

	while (it != pathlist.end()) {
		if (*it == "..") {//返回上级
			if (dest->getParent() == nullptr)
				return nullptr;
			dest = dest->getParent();
		}
		else if (*it != ".") {
			try {
				auto ff = dest->childs.at(*it);

				if (!ff)
					throw exception();

				if (!ff->isFolder())
					return nullptr;

				dest = (MiniFolder*)ff;

				if (!dest->isLoaded())
					dest->load();

			}
			catch (const std::exception&) {
				if (!queryCreateNew)
					return nullptr;

				std::string absPath;
				cout << dest->getAbsolutePath() << "/" + *it + " doesn't exist,create one?[Y/N]:";
				std::string in;
				getline(cin, in);

				if (in.length() == 1 && (in[0] == 'Y' || in[0] == 'y')) {
					auto folder = dest->createChildFolder(*it);
					dest->updateDir();
					dest = folder;
				}
				else
					return nullptr;
			}
		}

		it++;
	}

	return dest;

}