#include "stdafx.h"
#include "HandleTree.h"

REGISTER_HANDLER(HandleTree,"tree","�����η�ʽ�ݹ���ʾĿ¼","tree [\"filepath\"]","filepath����ѡ������Ŀ���ļ���")


void HandleTree::onHandleCommand(Lexer&param) {

	using namespace std;

	string pathname;

	param > pathname >= Lexer::end;

	vector<string>out;
	Helper::cutPathFromString(pathname, out);

	auto f = ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) {
		cout << f->getFilename() << '\n';
		f->printTree(0);
	}
	else
		cout << "Ŀ��Ŀ¼������\n";


}