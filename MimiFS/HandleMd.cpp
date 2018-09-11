#include "stdafx.h"
#include "HandleMd.h"

REGISTER_HANDLER(HandleMd, "md", "新建一个文件夹", "md \"path\"", "path:要创建文件夹的路径")

void HandleMd::onHandleCommand(Lexer&param) {

	using namespace std;

	string filepath;

	param >= filepath >= Lexer::end;

	vector<string>out;

	Helper::cutPathFromString(filepath, out);
	if (out.size()) {
		string filename = out.back();

		out.pop_back();

		auto f = ConsoleApp::getInstance()->getFolderByPath(out);

		if (!f) {
			cout << "创建失败\n";
		}

		auto child = f->getChild(filename);
		if (child) {
			cout << "目标文件已存在\n";
		}
		else {
			f->createChildFolder(filename);
			f->updateDir();
		}
		return;
	}
}
