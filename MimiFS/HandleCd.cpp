#include "stdafx.h"
#include "HandleCd.h"

REGISTER_HANDLER(HandleCd, "cd", "�л���ǰĿ¼", "cd \"path\"\npath:���·�����߾���·��")

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
		cout << "Ŀ��Ŀ¼������\n";
	

}