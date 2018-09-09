#include "stdafx.h"
#include "HandleCd.h"

REGISTER_HANDLER(HandleCd, "cd", "切换当前目录", "cd \"path\"\npath:相对路径或者绝对路径")

void HandleCd::onHandleCommand(Lexer&param) {

	using namespace std;

	string pathname;

	if (param.nextTokenMatchString()&&param.nextTokenMatchEnd()) {
		pathname = param.str;
	}
	else {
		cout << "cd \"path\"\n";
		return;
	}

	vector<string>out;
	Helper::cutPathFromString(pathname, out);

	auto f = ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) 
		ConsoleApp::getInstance()->current = f;
	else 
		cout << "目标目录不存在\n";
	

}