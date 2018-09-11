#include "stdafx.h"
#include "HandleTree.h"

REGISTER_HANDLER(HandleTree,"tree","以树形方式递归显示目录","tree [\"filepath\"]","filepath：可选参数，目标文件夹")


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
		cout << "目标目录不存在\n";


}