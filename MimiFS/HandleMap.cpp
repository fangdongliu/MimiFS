#include "stdafx.h"
#include "HandleMap.h"

REGISTER_HANDLER(HandleMap,"map","显示文件所占用块的\"块号\"","map filepath","filepath:要执行操作的文件")

HandleMap::HandleMap()
{
}


HandleMap::~HandleMap()
{
}

void HandleMap::onHandleCommand(Lexer&param) {

	using namespace std;

	string pathname;

	param >= pathname >= Lexer::end;

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
			std::vector<BlockHead>maps;
			b->getMap(maps);
			for (auto i : maps) {
				if (i.arraySize == 1) {
					printf("%d ", i.nextBlockId);
				}
				else {
					printf("(%d-%d) ", i.nextBlockId, i.nextBlockId + i.arraySize - 1);
				}
			}
			cout << '\n';
		}
		catch (exception&e) {
			cout << "目标文件不存在\n";
		}
	}
	else
		cout << "目标目录不存在\n";


}
