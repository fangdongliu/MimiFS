#include "stdafx.h"
#include "HandleCp.h"
#include "MiniFileReader.h"
#include "MiniFileWriter.h"

using namespace std;

REGISTER_HANDLER(HandleCp, "cp", "�����ļ�(��)��ָ��λ��", "cp \"filepath\" \"destpath\"", "filepath:ԭ�ļ�(��)·��\ndestpath:Ŀ��·��")

HandleCp::HandleCp()
{
	
}


HandleCp::~HandleCp()
{
}

void HandleCp::onHandleCommand(Lexer&param) {

	cnt1 = 0;
	cnt2 = 0;

	string filepath;
	string destpath;

	param >= filepath >= destpath >= Lexer::end;

	vector<string>pathlist;
	Helper::cutPathFromString(filepath, pathlist);

	if (!pathlist.size()) {
		return;
	}

	auto f = ConsoleApp::getInstance()->getFileByPath(pathlist);
	if (f == &ConsoleApp::getInstance()->root) {
		throw exception("���ܸ���root�ļ���");
	}

	Helper::cutPathFromString(destpath, pathlist);

	auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist);

	if (!folder) {
		throw exception("Ŀ���ļ�������");
	}

	if (f->isFolder()) {
		auto f2 = (MiniFolder*)f;
		if (folder->isChildOf(f2)) {
			throw exception("�������Ϸ�");
		}
	}

	std::string filename = f->getFilename();
	try {
		auto& b = folder->atChild(filename);

		if (b) {
			cout << "Ŀ���ļ�(��)�Ѵ��ڣ��Ƿ񸲸�?[Y/N]:";
			string str;
			getline(cin, str);
			if (str.length() == 1 && str[0] == 'y' || str[0] == 'Y') {
				b->deleteForever();
				delete b;
				b = nullptr;
			}
			else {
				cout<<"����ʧ��\n";
				return;
			}
		}
	}
	catch (exception&e) {

	}
	doCopy(f, folder);

	folder->updateDir();

	cout << "�����ļ���:" << cnt1 << '\n' << "�����ļ�����:" << cnt2<<'\n';

}

void HandleCp::doCopy(MiniFile*from,MiniFolder*dest) {

	string filename = from->getFilename();

	if (from->isFolder()) {
		cnt2++;
		auto folder = dest->createChildFolder(filename);

		vector<MiniFile*>matches;
		string pattern = "*";
		((MiniFolder*)from)->findMatchFiles(pattern, matches);

		for (auto i : matches) {
			doCopy(i, folder);
		}

		folder->updateDir();
	}
	else {
		cnt1++;
		auto file = dest->createChildFile(filename);

		MiniFileReader reader(from);

		MiniFileWriter writer(file);

		int maxlen = reader.getBlockMaxReadSize();

		char *buf = new char[maxlen + 5];

		int len=0;

		while (len = reader.readABlock(buf)) {
			writer.writeToBlock(buf, len);
		}

	}


}