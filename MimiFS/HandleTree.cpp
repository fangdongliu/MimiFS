#include "stdafx.h"
#include "HandleTree.h"

REGISTER_HANDLER(HandleTree,"tree","�����η�ʽ�ݹ���ʾĿ¼","tree [\"filepath\"]\nfilepath����ѡ������Ŀ���ļ���")


void HandleTree::onHandleCommand(Lexer&param) {

	using namespace std;

	string pathname;

	if (param.nextTokenMatchString()) {
		if (!param.nextTokenMatchEnd()) {
			cout << "tree \"path\"\n";
			return;
		}
		pathname = param.str;
	}
	else if (!param.matchEnd()) {
		cout << "tree \"path\"\n";
		return;
	}

	vector<string>out;
	Helper::cutPathFromString(pathname, out);

	auto f = ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) {
		cout << f->getFilename() << endl;
		f->printTree(0);
	}
	else
		cout << "Ŀ��Ŀ¼������\n";


}