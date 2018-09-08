#include "stdafx.h"
#include "HandleCd.h"

REGISTER_HANDLER(HandleCd, "cd", "�л���ǰĿ¼", "cd [\"path\"]\npath:��ѡ���������·�����߾���·��")

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
		cout << "Ŀ��Ŀ¼������\n";
	}



}