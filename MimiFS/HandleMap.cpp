#include "stdafx.h"
#include "HandleMap.h"

REGISTER_HANDLER(HandleMap,"map","��ʾ�ļ���ռ�ÿ��\"���\"","map filepath","filepath:Ҫִ�в������ļ�")

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
				cout << "�ļ���:" << b->getFilename() << " ��ռ�̿�Ϊ��\n";
			}
			else {
				cout << "�ļ�:" << b->getFilename() << " ��ռ�̿�Ϊ��\n";
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
			cout << "Ŀ���ļ�������\n";
		}
	}
	else
		cout << "Ŀ��Ŀ¼������\n";


}
