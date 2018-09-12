#include "stdafx.h"
#include "HandleMf.h"

REGISTER_HANDLER(HandleMf, "mf", "创建一个新的文件", "mf \"filepath\"","filepath:要创建文件的路径")

void HandleMf::onHandleCommand(Lexer&param) {

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
			f->createChildFile(filename,1);
			f->updateDir();
		}
	}


}