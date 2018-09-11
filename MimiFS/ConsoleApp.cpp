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

	helpList["close"] = { "�ر�MiniFSϵͳ","close" };
	helpList["create"] = { "����һ��MiniFSϵͳ","create \"filename\" [fileSize] [blockSize]","filename : Ҫ����ϵͳ���ļ���\nfileSize : ϵͳ���ļ���С\nblockSize : ϵͳÿ����Ĵ�С" };
	helpList["mount"] = { "װ��һ��MiniFSϵͳ","mount \"filename\"","filename : Ҫװ��ϵͳ���ļ���" };
	helpList["help"] = { "��ʾ����","help [\"cmd\"]","cmd : ��ѡ��������ָ����鿴����ϸ����������г���������" };
	helpList["fmt"] = { "��ʽ��MiniFSϵͳ","fmt [fileSize] [blockSize]","fileSize : ϵͳ���ļ���С\nblockSize : ϵͳÿ����Ĵ�С" };
	helpList["info"] = { "��ʾMiniFSϵͳ״̬��Ϣ","info","" };

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
		else if (!ready()) //�ж�ϵͳ�Ƿ����
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
		cout << "the right usage : " << helpList[lexer.getName()].format << '\n';
	}
	catch (const InvalidFilename&e) {
		cout << "���Ϸ����ļ���\n";
	}
	catch (const std::exception& e) {
		cout << e.what() << '\n';
	}
}

void ConsoleApp::showFSInfo(Lexer&param) {

	param >= Lexer::end;

	cout << "ϵͳ�� : " + MiniFile::op.filename << '\n';
	cout << "���п����" << MiniFile::op.superHead.emptyBlockCount << '\n';
	cout << "���п�ͷ��Id:" << MiniFile::op.superHead.firstEmptyBlockId << "(0/1/2�����)" << '\n';

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
			cout << "Ŀ���������" << '\n';
		}
	}
	else {
		string out;

		std::vector<std::pair<std::string,HelpItem>>strs(helpList.begin(),helpList.end());
		std::sort(strs.begin(), strs.end(), [](std::pair<std::string, HelpItem>&a, std::pair<std::string, HelpItem>&b)->bool {
			return a.first < b.first;
		});

		char buf[100];

		for (auto i : strs) {
			sprintf_s(buf, "%- 8s : %- 24s %- 40s\n",i.first.c_str(),i.second.title.c_str(),i.second.format.c_str());
			printf(buf);
		}
	}

}

void ConsoleApp::closeMiniFS(Lexer&param) {

	param >= Lexer::end;

	MiniFile::op.close();
	root.clear();
	current = &root;
	std::cout << "miniFS ϵͳ( " + MiniFile::op.filename + " )�ر�" << '\n';

}

void ConsoleApp::formatMiniFS(Lexer& param) {

	SuperHead head;
	head.fileSize = 1 << 30;
	head.blockSize = 1 << 10;

	param > head.fileSize > head.blockSize >= Lexer::end;

	head.firstEmptyBlockId = 3;
	head.emptyBlockCount = head.fileSize / head.blockSize - 2;

	auto &f = MiniFile::op;

	f.superHead = head;//д���ļ�ͷ
	f.seekBlock(0);
	f.write(head);

	BlockHead blockHead;
	blockHead.nextBlockId = 0;
	blockHead.size = 0;

	f.seekBlock(1);		//����root�ļ���
	f.write(blockHead);

	f.seekBlock(2);		//���ǻ���վ
	f.write(blockHead);

	//����������
	for (int i = 3; i < head.emptyBlockCount + 3; i++) {
		f.seekBlock(i);
		blockHead.nextBlockId = i + 1;
		f.write(blockHead);
	}

	blockHead.nextBlockId = 0;

	f.seekBlock(head.emptyBlockCount + 3);//������β������
	f.write(blockHead);

	root.clear();//�����ڴ��Ŀ¼�뵱ǰĿ¼
	current = &root;

	std::cout << "miniFS ϵͳ( " + f.filename + " )��ʽ�����" << '\n';
}

void ConsoleApp::mountMiniFS(Lexer& param) {

	string filename;

	param >= filename >= Lexer::end;

	//�ر���װ��ϵͳ
	if (ready()) {
		string close = "";
		Lexer closeParam(close);
		closeMiniFS(closeParam);
	}

	//��ȫ.fs��׺
	if (filename.length() < 3)
		filename += ".fs";
	else if (filename.substr(filename.length() - 3) != ".fs")
		filename += ".fs";

	MiniFile::op.open(filename);


	if (!ready()) {
		std::cout << "�ļ�:" << filename << " �����ڻ�ռ��" << '\n';
		return;
	}

	root.load();//����rootĿ¼
	current = &root;//���õ�ǰ�ļ���
	std::cout << "miniFS ϵͳ( " + filename + " )�������" << '\n';

}

void ConsoleApp::createMiniFS(Lexer& param) {

	std::string filename;
	SuperHead head;
	head.fileSize = 1 << 30;
	head.blockSize = 1 << 10;

	param >= filename > head.fileSize > head.blockSize >= Lexer::end;

	//��ȫ.fs��׺
	if (filename.length() < 3)
		filename += ".fs";
	else if (filename.substr(filename.length() - 3) != ".fs")
		filename += ".fs";

	MyFileWriter writer(filename);
	if (!writer.ready()) {
		return;
	}
	head.firstEmptyBlockId = 3;
	head.emptyBlockCount = head.fileSize / head.blockSize - 2;

	writer.write(head);//д���ļ�ͷ
	writer.setBlockSize(head.blockSize);

	BlockHead blockHead;
	blockHead.nextBlockId = 0;
	blockHead.size = 0;

	writer.seekBlock(1);//д��rootĿ¼
	writer.write(blockHead);

	writer.seekBlock(2);//д�����վ
	writer.write(blockHead);

	//����������
	for (int i = 3; i < head.emptyBlockCount + 3; i++) {
		writer.seekBlock(i);
		blockHead.nextBlockId = i + 1;
		writer.write(blockHead);
	}

	//������β��
	blockHead.nextBlockId = 0;
	writer.seekBlock(head.emptyBlockCount + 3);
	writer.write(blockHead);

	std::cout << "miniFS ϵͳ( " + filename + " )�������" << '\n';
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
			return folder->atChild(filename);
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

	if (*it == "root:") {//����·��
		dest = &root;
		it++;
	}

	while (it != pathlist.end()) {
		if (*it == "..") {//�����ϼ�
			if (dest->getParent() == nullptr)
				return nullptr;
			dest = dest->getParent();
		}
		else if (*it != ".") {
			try{
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