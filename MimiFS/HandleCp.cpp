#include "stdafx.h"
#include "HandleCp.h"
#include "MiniFileReader.h"
#include "MiniFileWriter.h"

using namespace std;

REGISTER_HANDLER(HandleCp, "cp", "复制文件(夹)至指定位置", "cp \"filepath\" \"destpath\"", "filepath:原文件(夹)路径\ndestpath:目标路径")

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
		throw exception("不能复制root文件夹");
	}

	Helper::cutPathFromString(destpath, pathlist);

	auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist);

	if (!folder) {
		throw exception("目标文件不存在");
	}

	if (f->isFolder()) {
		auto f2 = (MiniFolder*)f;
		if (folder->isChildOf(f2)) {
			throw exception("操作不合法");
		}
	}

	std::string filename = f->getFilename();
	try {
		auto& b = folder->atChild(filename);

		if (b) {
			cout << "目标文件(夹)已存在，是否覆盖?[Y/N]:";
			string str;
			getline(cin, str);
			if (str.length() == 1 && str[0] == 'y' || str[0] == 'Y') {
				b->deleteForever();
				delete b;
				b = nullptr;
			}
			else {
				cout<<"复制失败\n";
				return;
			}
		}
	}
	catch (exception&e) {

	}
	doCopy(f, folder);

	folder->updateDir();

	cout << "复制文件数:" << cnt1 << '\n' << "复制文件夹数:" << cnt2<<'\n';

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