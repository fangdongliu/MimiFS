#include "stdafx.h"
#include "HandleMd.h"

REGISTER_HANDLER(HandleMd, "md", "�½��ļ���", "md [\"path\"]\npath:��ѡ���������·�����߾���·��")

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
		cout << "����ʧ��\n";
	}

	auto child = f->getChild(filename);
	if (child) {
		cout << "Ŀ���ļ��Ѵ���\n";
	}
	else {
		f->createChildFolder(filename);
		f->updateDir();
	}
}
