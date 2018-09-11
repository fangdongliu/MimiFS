#include "stdafx.h"
#include "HandleMd.h"

REGISTER_HANDLER(HandleMd, "md", "�½�һ���ļ���", "md \"path\"", "path:Ҫ�����ļ��е�·��")

void HandleMd::onHandleCommand(Lexer&param) {

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
		return;
	}
}
