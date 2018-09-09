#include "stdafx.h"
#include "HandleRename.h"

REGISTER_HANDLER(HandleRename, "rename", "修改文件(夹)名", "rename \"filepath\" \"destname\"\nfilepath:要修改文件的路径\ndestname:目标文件名")

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

		try {

			auto& b = folder->atChild(filename);

			if (!b) {
				throw exception();
			}

			auto& e = folder->getChild(to);
			if (e) {
				cout << "目标文件已存在，是否覆盖?[Y/N]:";
				string str;
				getline(cin, str);
				if (str.length() == 1 && str[0] == 'y' || str[0] == 'Y') {
					e->deleteForever();
					delete e;
				}
				else {
					cout << "修改失败";
					return;
				}
			}

			b->setFilename(to);
			e = b;
			b = nullptr;
			folder->updateDir();
		}
		catch (exception&e) {
			cout << "目标文件不存在";
		}

		return;
	}


renameError:
	cout << "rename \"filepath\" \"destname\"\n";
}