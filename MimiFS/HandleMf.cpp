#include "stdafx.h"
#include "HandleMf.h"

REGISTER_HANDLER(HandleMf, "mf", "����һ���µ��ļ�", "mf \"filepath\"","filepath:Ҫ�����ļ���·��")

void HandleMf::onHandleCommand(Lexer&param) {

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
			f->createChildFile(filename,1);
			f->updateDir();
		}
	}


}