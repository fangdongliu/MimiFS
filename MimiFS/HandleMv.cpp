#include "stdafx.h"
#include "HandleMv.h"

REGISTER_HANDLER(HandleMv, "mv", "移动文件(夹)至指定路径", "mv \"filepath\"\"path\"", "filepath:要移动的文件(夹)名\nfilepath:目标路径")

HandleMv::HandleMv()
{
}


HandleMv::~HandleMv()
{
}

void HandleMv::onHandleCommand(Lexer&param) {

	using namespace std;

	string from, to;

	param >= from >= to >= Lexer::end;

	std::vector<std::string>pathlist;
	Helper::cutPathFromString(from, pathlist);
	if (!pathlist.size()) {
		cout << "未找到文件夹\n";
		return;
	}
	std::string filename = pathlist.back();
	pathlist.pop_back();

	auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
	if (!folder) {
		cout << "未找到文件夹\n";
		return;
	}

	Helper::cutPathFromString(to, pathlist);
	if (!pathlist.size()) {
		cout << "未找到目标文件夹\n";
		return;
	}

	auto folder2 = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
	if (!folder2) {
		cout << "未找到目标文件夹\n";
		return;
	}

	if (folder2->moveFrom(folder, filename)) {
		cout << "移动成功\n";
	}
	else cout << "移动失败\n";
}