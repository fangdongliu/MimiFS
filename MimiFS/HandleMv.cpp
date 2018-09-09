#include "stdafx.h"
#include "HandleMv.h"

REGISTER_HANDLER(HandleMv, "mv", "�ƶ��ļ�(��)��ָ��·��", "mv \"filepath\"\"path\"\nfilepath:Ҫ�ƶ����ļ�(��)��\nfilepath:Ŀ��·��")

HandleMv::HandleMv()
{
}


HandleMv::~HandleMv()
{
}

void HandleMv::onHandleCommand(Lexer&param) {

	using namespace std;

	string from, to;
	if (!param.nextTokenMatchString()) {
		goto mvError;
	}
	from = param.str;

	if (!param.nextTokenMatchString()) {
		goto mvError;
	}
	to = param.str;

	if (!param.nextTokenMatchEnd())
		goto mvError;

	{
		std::vector<std::string>pathlist;
		Helper::cutPathFromString(from, pathlist);
		if (!pathlist.size()) {
			cout << "δ�ҵ��ļ���\n";
			return;
		}
		std::string filename = pathlist.back();
		pathlist.pop_back();

		auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
		if (!folder) {
			cout << "δ�ҵ��ļ���\n";
			return;
		}
		
		Helper::cutPathFromString(to, pathlist);
		if (!pathlist.size()) {
			cout << "δ�ҵ�Ŀ���ļ���\n";
			return;
		}

		auto folder2 = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
		if (!folder2) {
			cout << "δ�ҵ�Ŀ���ļ���\n";
			return;
		}

		if (folder2->moveFrom(folder, filename)) {
			cout << "�ƶ��ɹ�\n";
		}
		else cout << "�ƶ�ʧ��\n";
		return;

	}
mvError:
	cout << "mv \"filepath\"\"path\"";
}