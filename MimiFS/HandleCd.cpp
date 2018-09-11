#include "stdafx.h"
#include "HandleCd.h"

using namespace std;

REGISTER_HANDLER(HandleCd, "cd", "切换当前目录", "cd \"path\"","path:相对路径或者绝对路径")

void HandleCd::onHandleCommand(Lexer&param) {

	string pathname;

	param > pathname >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	vector<string>out;
	Helper::cutPathFromString(pathname, out);

	auto f = ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) 
		ConsoleApp::getInstance()->current = f;
	else 
		cout << "目标目录不存在\n";
	

}