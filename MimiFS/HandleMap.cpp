#include "stdafx.h"
#include "HandleMap.h"

REGISTER_HANDLER(HandleMap,"map","��ʾָ���ļ��ڿռ���ռ�õ�ȫ�����\"���\"","map filepath\nfilepath:Ҫִ�в������ļ�")

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
				cout << "�ļ���:" << b->getFilename() << " ��ռ�̿�Ϊ��\n";
			}
			else {
				cout << "�ļ�:" << b->getFilename() << " ��ռ�̿�Ϊ��\n";
			}
			b->showMap();
			cout << endl;
		}
		catch (exception&e) {
			cout << "Ŀ���ļ�������\n";
		}
	}
	else
		cout << "Ŀ��Ŀ¼������\n";


}
