#include "stdafx.h"
#include "HandleMap.h"

REGISTER_HANDLER(HandleMap,"map","显示指定文件在空间所占用的全部块的\"块号\"","map filepath\nfilepath:要执行操作的文件")

HandleMap::HandleMap()
{
}


HandleMap::~HandleMap()
{
}

void HandleMap::onHandleCommand(Lexer&param) {

	using namespace std;

	string pathname;

	if (param.nextTokenMatchString() && param.nextTokenMatchEnd()) {
		pathname = param.str;
	}
	else {
		cout << "map \"filepath\"\n";
		return;
	}

	vector<string>out;
	Helper::cutPathFromString(pathname, out);

	string filename = out.back();
	out.pop_back();

	auto f = ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) {
		try {
			auto b = f->atChild(filename);
			if (b->isFolder()) {
				cout << "文件夹:" << b->getFilename() << " 所占盘块为：\n";
			}
			else {
				cout << "文件:" << b->getFilename() << " 所占盘块为：\n";
			}
			b->showMap();
			cout << endl;
		}
		catch (exception&e) {
			cout << "目标文件不存在\n";
		}
	}
	else
		cout << "目标目录不存在\n";


}
