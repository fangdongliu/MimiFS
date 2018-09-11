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

	helpList["close"] = { "关闭MiniFS系统","" };
	helpList["create"] = { "创建一个MiniFS系统","create \"filename\" [fileSize] [blockSize]","filename : 要创建系统的文件名\nfileSize : 系统的文件大小\nblockSize : 系统每个块的大小" };
	helpList["mount"] = { "装载一个MiniFS系统","mount \"filename\"","filename : 要装载系统的文件名" };
	helpList["help"] = { "显示帮助","help [\"cmd\"]","cmd : 可选参数，若指定则查看其详细情况，否则列出所有命令" };
	helpList["fmt"] = { "格式化MiniFS系统","fmt [fileSize] [blockSize]","fileSize : 系统的文件大小\nblockSize : 系统每个块的大小" };
	helpList["info"] = { "显示MiniFS系统状态信息","info","" };

}

bool ConsoleApp::ready() {
	return MiniFile::op.ready();
}

void ConsoleApp::printPrefix() {

	if (MiniFile::op.ready()) {
		/*if (current == &trashBin) {
			std::cout << "trashBin:/ > ";
		}
		else */ {
			std::cout << current->getAbsolutePath() + " > ";
		}
	}
	else {
		std::cout << "MimiFS > ";
	}

}

void ConsoleApp::handleCommand(Lexer&lexer) {

	try {
		if (lexer.getName() == "mount")
			mountMiniFS(lexer);
		else if (lexer.getName() == "create")
			createMiniFS(lexer);
		else if (lexer.getName() == "help")
			showHelp(lexer);
		else if (!ready())
			cout << "use \"mount\" to load a miniFS-space OR use \"create\" to create a miniFS-space\n";
		else if (lexer.getName() == "fmt")
			formatMiniFS(lexer);
		else if (lexer.getName() == "close")
			closeMiniFS(lexer);
		else if (lexer.getName() == "info")
			showFSInfo(lexer);
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
		cout << "the right usage : " << helpList[lexer.getName()].format<<'\n';
	}
	catch (const InvalidFilename&e) {
		cout << "不合法的文件名\n";
	}
	catch (const std::exception& e) {
		cout << e.what() << '\n';
	}
}

void ConsoleApp::showFSInfo(Lexer&param) {

	param >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	cout << "系统名 : " + MiniFile::op.filename << '\n';
	cout << "空闲块个数" << MiniFile::op.superHead.emptyBlockCount << '\n';
	cout << "空闲块头部Id:" << MiniFile::op.superHead.firstEmptyBlockId << "(0/1/2则出错)" << '\n';

}

void ConsoleApp::showHelp(Lexer& param) {

	string cmd;

	param > cmd >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

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
		for (auto i : helpList) {
			out += i.first;
			out += ' : ';
			out += i.second.title;
			out += ' ';
			out += i.second.format;
			out += '\n';
		}
		printf("%s",out.c_str());
	}

}

void ConsoleApp::closeMiniFS(Lexer&param) {

	param >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();


	MiniFile::op.close();
	root.clear();
	current = &root;
	std::cout << "miniFS 系统( " + MiniFile::op.filename + " )关闭" << '\n';

}

void ConsoleApp::formatMiniFS(Lexer& param) {

	SuperHead head;
	head.fileSize = 1 << 30;
	head.blockSize = 1 << 10;

	param > head.fileSize > head.blockSize >= Lexer::end;

	head.firstEmptyBlockId = 3;
	head.emptyBlockCount = head.fileSize / head.blockSize - 2;

	auto &f = MiniFile::op;

	f.superHead = head;
	f.seekBlock(0);
	f.write(head);

	BlockHead blockHead;
	blockHead.nextBlockId = 0;
	blockHead.size = 0;

	f.seekBlock(1);
	f.write(blockHead);

	f.seekBlock(2);
	f.write(blockHead);

	for (int i = 3; i < head.emptyBlockCount + 3; i++) {
		f.seekBlock(i);
		blockHead.nextBlockId = i + 1;
		f.write(blockHead);
	}

	f.seekBlock(head.emptyBlockCount + 3);

	blockHead.nextBlockId = 0;
	f.write(blockHead);

	root.clear();
	current = &root;

	std::cout << "miniFS 系统( " + f.filename + " )格式化完成" << '\n';
}

void ConsoleApp::mountMiniFS(Lexer& param) {

	string filename;

	param >= filename >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	if (ready()) {
		string close = "";
		Lexer closeParam(close);
		closeMiniFS(closeParam);
	}

	if (filename.length() < 3) {
		filename += ".fs";
	}
	else if (filename.substr(filename.length() - 3) != ".fs") {
		filename += ".fs";
	}


	MiniFile::op.open(filename);

	if (!MiniFile::op.ready()) {
		std::cout << "文件:" << filename << " 不存在或被占用" << '\n';
		return;
	}

	root.load();
	current = &root;
	std::cout << "miniFS 系统( " + filename + " )加载完毕" << '\n';

}

void ConsoleApp::createMiniFS(Lexer& param) {

	std::string filename;
	SuperHead head;
	head.fileSize = 1 << 30;
	head.blockSize = 1 << 10;

	param >= filename > head.fileSize > head.blockSize >= Lexer::end;

	if (filename.length() < 3) {
		filename += ".fs";
	}
	else if (filename.substr(filename.length() - 3) != ".fs") {
		filename += ".fs";
	}

	MyFileWriter writer(filename);
	if (!writer.ready()) {
		return;
	}
	head.firstEmptyBlockId = 3;
	head.emptyBlockCount = head.fileSize / head.blockSize - 2;

	writer.write(head);
	writer.setBlockSize(head.blockSize);

	BlockHead blockHead;
	blockHead.nextBlockId = 0;
	blockHead.size = 0;

	writer.seekBlock(1);

	writer.write(blockHead);

	writer.seekBlock(2);

	writer.write(blockHead);

	for (int i = 3; i < head.emptyBlockCount + 3; i++) {

		writer.seekBlock(i);

		blockHead.nextBlockId = i + 1;
		writer.write(blockHead);

	}

	writer.seekBlock(head.emptyBlockCount + 3);

	blockHead.nextBlockId = 0;
	writer.write(blockHead);

	std::cout << "miniFS 系统( " + filename + " )创建完成" << '\n';
}

MiniFile* ConsoleApp::getFileByPath(std::vector<std::string>&pathList) {
	
	string filename = pathList.back();
	pathList.pop_back();

	if (auto folder=getFolderByPath(pathList, false)) {

		if (filename == ".") {
			return folder;
		}
		else if (filename == "..") {
			return folder->parent;
		}
		else {
			return folder->atChild(filename);
		}
	}
	else {
		throw PathNotExist();
	}

}

MiniFolder* ConsoleApp::getFolderByPath(std::vector<std::string>&pathlist, bool queryCreateNew) {

	using namespace std;

	MiniFolder* dest = current;

	auto it = pathlist.begin();

	if (it == pathlist.end())
		return dest;

	if (*it == "root:") {
		dest = &root;
		it++;
	}
	/*else if (*pathlist.begin() == "trashBin:"&&pathlist.size() == 1) {
		return &trashBin;
	}*/

	while (it != pathlist.end()) {
		if (*it == "..") {
			if (dest->getParent() == nullptr) {
				return nullptr;
			}
			dest = dest->getParent();
		}
		else if (*it != ".") {
			try
			{
				auto ff = dest->childs.at(*it);

				if (!ff) {
					if (!queryCreateNew) {
						return nullptr;
					}
					{
						std::string absPath;
						cout << dest->getAbsolutePath() << "/" + *it + " doesn't exist,create one?[Y/N]:";
						std::string in;
						getline(cin, in);

						if (in.length() > 0 && (in[0] == 'Y' || in[0] == 'y')) {
							auto folder = dest->createChildFolder(*it);
							dest->updateDir();
							dest = folder;
						}
						else {
							return nullptr;
						}
					}
				}
				else {
					if (!ff->isFolder())
						return nullptr;

					dest = (MiniFolder*)ff;
					if (!dest->isLoaded()) {
						dest->load();
					}
				}
			}
			catch (const std::exception&)
			{
				if (!queryCreateNew) {
					return nullptr;
				}
				{
					std::string absPath;
					cout << dest->getAbsolutePath() << "/" + *it + " doesn't exist,create one?[Y/N]:";
					std::string in;
					getline(cin, in);

					if (in.length() > 0 && (in[0] == 'Y' || in[0] == 'y')) {
						auto folder = dest->createChildFolder(*it);
						dest->updateDir();
						dest = folder;
					}
					else {
						return nullptr;
					}
				}
			}
		}

		it++;
	}

	return dest;

}