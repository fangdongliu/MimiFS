#include "stdafx.h"
#include "ConsoleApp.h"
#include "CommandHandler.h"
ConsoleApp* ConsoleApp::instance = nullptr;

ConsoleApp::ConsoleApp() {

	helpList["close"] = { "�ر�MiniFSϵͳ","" };
	helpList["create"] = { "����һ��MiniFSϵͳ","create \"filename\" [fileSize] [blockSize]\nfilename : Ҫ����ϵͳ���ļ���\nfileSize : ϵͳ���ļ���С\nblockSize : ϵͳÿ����Ĵ�С" };
	helpList["mount"] = { "װ��һ��MiniFSϵͳ","mount \"filename\"\nfilename : Ҫװ��ϵͳ���ļ���" };
	helpList["help"] = { "��ʾ����","help [\"cmd\"]\ncmd : ��ѡ��������ָ����鿴����ϸ����������г���������" };
	helpList["fmt"] = { "��ʽ��MiniFSϵͳ","fmt [fileSize] [blockSize]\nfileSize : ϵͳ���ļ���С\nblockSize : ϵͳÿ����Ĵ�С" };
	helpList["info"] = { "��ʾMiniFSϵͳ״̬��Ϣ","" };

}

void ConsoleApp::printPrefix() {

	if (MiniFile::op.ready()) {
		if (current == &trashBin) {
			std::cout << "trashBin:/ > ";
		}
		else {
			std::cout << current->getAbsolutePath() + " > ";
		}
	}
	else {
		std::cout << "MimiFS > ";
	}

}

void ConsoleApp::handleCommand(std::string&command) {

	using namespace std;

	Lexer lexer(command);

	switch (lexer.nextToken())
	{
	case Lexer::Token::None:
		printPrefix();
		return;
	case Lexer::Token::String:
		break;
	default:
		cout << "wrong command" << std::endl;
		goto handleCommandEnd;
	}

	if (lexer.str == "mount") {
		string no = "";
		Lexer param2(no);
		if (MiniFile::op.ready())
			closeMiniFS(param2);
		mountMiniFS(lexer);
	}
	else if (lexer.str == "create") {
		createMiniFS(lexer);
	}
	else if (lexer.str == "help") {
		showHelp(lexer);
	}
	else if (MiniFile::op.ready()) {
		if (lexer.str == "fmt") {
			formatMiniFS(lexer);

		}
		else if (lexer.str == "close") {
			closeMiniFS(lexer);
		}
		else if (lexer.str == "info") {
			showFSInfo(lexer);
		}
		else {
			CommandHandler *dealer = nullptr;

			try
			{
				dealer = handler.at(lexer.str);
			}
			catch (const std::exception& e)
			{
				cout << "wrong command" << std::endl;
				goto handleCommandEnd;
			}

			try
			{
				dealer->onHandleCommand(lexer);
			}
			catch (const std::exception& e)
			{
				cout << e.what() << std::endl;
			}
		}
		if (MiniFile::op.ready())
			MiniFile::op.updateHead();
	}
	else {
		cout << "use \"mount\" to load a miniFS-space OR use \"create\" to create a miniFS-space" << std::endl;
	}
handleCommandEnd:

	cout << std::endl;
	printPrefix();

}

void ConsoleApp::showFSInfo(Lexer&param) {
	
	using namespace std;

	if (param.nextToken() != Lexer::Token::None) {
		std::cout << "info need no param";
		return;
	}

	cout << "ϵͳ�� : " + MiniFile::op.filename<<endl;
	cout << "���п����" << MiniFile::op.superHead.emptyBlockCount << endl;
	cout << "���п�ͷ��Id:" << MiniFile::op.superHead.firstEmptyBlockId << "(0/1/2�����)" << endl;

}

void ConsoleApp::showHelp(Lexer& param) {

	using namespace std;

	if (param.nextToken() == Lexer::Token::None) {
		for (auto i : helpList) {
			cout << i.first << " : ";
			cout << i.second.title << endl;
		}
	}
	else if (param.token == Lexer::Token::String) {
		try
		{
			auto i = helpList.at(param.str);
			cout << i.title << endl;
			cout << i.detail << endl;
		}
		catch (const std::exception&)
		{
			cout << "Ŀ���������" << endl;
		}
	}
	else {
		cout << "help [\"command\"]\n";
	}

}

void ConsoleApp::closeMiniFS(Lexer&param) {

	if (param.nextToken() != Lexer::Token::None) {
		std::cout << "close need no param";
		return;
	}

	MiniFile::op.close();
	trashBin.clear();
	root.clear();
	current = &root;
	std::cout << "miniFS ϵͳ( " + MiniFile::op.filename + " )�ر�" << std::endl;

}

void ConsoleApp::formatMiniFS(Lexer& param) {

	using namespace std;

	SuperHead head;
	head.fileSize = 1 << 30;
	head.blockSize = 1 << 10;

	switch (param.nextToken())
	{
	case Lexer::Token::None:
		break;
	case Lexer::Token::Num:
		head.fileSize = param.num;

		switch (param.nextToken())
		{
		case Lexer::Token::None:
			break;
		case Lexer::Token::Num:
			head.blockSize = param.num;
			break;
		default:
			goto fmtMiniFSERR;
		}

		break;
	default:
		goto fmtMiniFSERR;
	}

	{

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
		trashBin.clear();
		current = &root;

		std::cout << "miniFS ϵͳ( " + f.filename + " )��ʽ�����" << std::endl;
		return;
	}
fmtMiniFSERR:
	std::cout << "fmt [filesize] [blocksize]" << std::endl;
}

void ConsoleApp::mountMiniFS(Lexer& param) {

	std::string filename;

	switch (param.nextToken())
	{
	case Lexer::Token::RealString:
	case Lexer::Token::String:
		filename = param.str;
		break;
	default:
		goto mountMiniFSERR;
	}

	{
		if (filename.length() < 3) {
			filename += ".fs";
		}
		else if (!(filename[filename.length() - 1] == 's'&&filename[filename.length() - 2] == 'f'&&filename[filename.length() - 3] == '.')) {
			filename += ".fs";
		}

		MiniFile::op.open(filename);

		if (!MiniFile::op.ready()) {
			std::cout << "�ļ�:" << filename << " �����ڻ�ռ��" << std::endl;
			return;
		}

		root.load();
		trashBin.load();
		current = &root;
		std::cout << "miniFS ϵͳ( " + filename + " )�������" << std::endl;

		return;
	}

mountMiniFSERR:
	std::cout << "mount \"filename\"" << std::endl;
}

void ConsoleApp::createMiniFS(Lexer& param) {

	std::string filename;

	switch (param.nextToken())
	{
	case Lexer::Token::RealString:
	case Lexer::Token::String:
		filename = param.str;
		break;
	default:
		goto createMiniFSERR;
	}

	{
		if (filename.length() < 3) {
			filename += ".fs";
		}
		else if (!(filename[filename.length() - 1] == 's'&&filename[filename.length() - 2] == 'f'&&filename[filename.length() - 3] == '.')) {
			filename += ".fs";
		}

		MyFileWriter writer(filename);
		if (!writer.ready()) {
			return;
		}

		SuperHead head;
		head.fileSize = 1 << 30;
		head.blockSize = 1 << 10;

		switch (param.nextToken())
		{
		case Lexer::Token::None:
			break;
		case Lexer::Token::Num:
			head.fileSize = param.num;

			switch (param.nextToken())
			{
			case Lexer::Token::None:
				break;
			case Lexer::Token::Num:
				head.blockSize = param.num;
				break;
			default:
				goto createMiniFSERR;
			}

			break;
		default:
			goto createMiniFSERR;
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

		std::cout << "miniFS ϵͳ( " + filename + " )�������" << std::endl;
		return;
	}

createMiniFSERR:
	std::cout << "create \"filename\" [filesize] [blocksize]" << std::endl;

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
	else if (*pathlist.begin() == "trashBin:"&&pathlist.size() == 1) {
		return &trashBin;
	}

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