#include "stdafx.h"
#include "HandleCd.h"

REGISTER_HANDLER(HandleCd, "cd", "�л���ǰĿ¼", "cd \"path\"\npath:���·�����߾���·��")

void HandleCd::onHandleCommand(Lexer&param) {

	using namespace std;

	string pathname;

	switch (param.nextToken())
	{
	case Lexer::Token::RealString:
	case Lexer::Token::String:
		pathname = param.str;
		if (param.nextToken() == Lexer::Token::None) {
			break;
		}
	default:
		cout << "cd \"path\"\n";
		return;
	}

	vector<string>out;

	Helper::cutPathFromString(pathname, out);

	auto f = ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) {
		ConsoleApp::getInstance()->current = f;
	}
	else {
		cout << "Ŀ��Ŀ¼������\n";
	}



}