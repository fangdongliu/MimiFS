#include "stdafx.h"
#include "HandleCd.h"

using namespace std;

REGISTER_HANDLER(HandleCd, "cd", "�л���ǰĿ¼", "cd \"path\"","path:���·�����߾���·��")

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
		cout << "Ŀ��Ŀ¼������\n";
	

}