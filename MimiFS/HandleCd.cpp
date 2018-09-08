#include "stdafx.h"
#include "HandleCd.h"

REGISTER_HANDLER(HandleCd, "cd", "切换当前目录", "cd [\"path\"]\npath:可选参数，相对路径或者绝对路径")

void HandleCd::onHandleCommand(std::istream&param) {

	using namespace std;

	string pathname;

	param >> pathname;

	vector<string>out;

	Helper::cutPathFromString(pathname, out);

	auto f = ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) {
		ConsoleApp::getInstance()->current = f;
	}
	else {
		cout << "目标目录不存在\n";
	}



}