#include "stdafx.h"
#include "HandleRename.h"

REGISTER_HANDLER(HandleRename, "rename", "�޸��ļ�(��)��", "rename \"filepath\" \"destname\"\nfilepath:Ҫ�޸��ļ���·��\ndestname:Ŀ���ļ���")

HandleRename::HandleRename()
{
}


HandleRename::~HandleRename()
{
}

void HandleRename::onHandleCommand(Lexer&param) {

	using namespace std;

	string from, to;
	if (!param.nextTokenMatchString()) {
		goto renameError;
	}
	from = param.str;

	if (!param.nextTokenMatchString()) {
		goto renameError;
	}
	to = param.str;

	if (!param.nextTokenMatchEnd())
		goto renameError;

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

		try {

			auto& b = folder->atChild(filename);

			if (!b) {
				throw exception();
			}

			auto& e = folder->getChild(to);
			if (e) {
				cout << "Ŀ���ļ��Ѵ��ڣ��Ƿ񸲸�?[Y/N]:";
				string str;
				getline(cin, str);
				if (str.length() == 1 && str[0] == 'y' || str[0] == 'Y') {
					e->deleteForever();
					delete e;
				}
				else {
					cout << "�޸�ʧ��";
					return;
				}
			}

			b->setFilename(to);
			e = b;
			b = nullptr;
			folder->updateDir();
		}
		catch (exception&e) {
			cout << "Ŀ���ļ�������";
		}

		return;
	}


renameError:
	cout << "rename \"filepath\" \"destname\"\n";
}