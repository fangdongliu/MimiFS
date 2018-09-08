#include "stdafx.h"
#include "HandleMd.h"

REGISTER_HANDLER(HandleMd, "md", "新建文件夹", "md [\"path\"]\npath:可选参数，相对路径或者绝对路径")

void HandleMd::onHandleCommand(std::istream&param) {

	using namespace std;



	string pathname;

	param >> pathname;
	

	vector<string>out;

	Helper::cutPathFromString(pathname, out);

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
}
