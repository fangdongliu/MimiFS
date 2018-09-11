#include "stdafx.h"
#include "HandleDr.h"

using namespace std;

REGISTER_HANDLER(HandleDr, "dr", "显示文件夹目录", "dr [-b] [\"path\"]", "[-b]:可选参数，开启模糊匹配\npath:可选参数，相对路径或者绝对路径")


void HandleDr::onHandleCommand(Lexer&param) {

	string folderpath;

	param > folderpath >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	bool blur = param.findSwitch("b");

	vector<string>out;
	Helper::cutPathFromString(folderpath, out);
	if (blur) {
		string filename;

		if (out.size()) {
			filename = out.back();
			out.pop_back();

			auto folder = ConsoleApp::getInstance()->getFolderByPath(out);

			if (folder) {
				std::vector<MiniFile*>files;
				folder->findMatchFiles(filename, files);
				cout << "匹配文件数：" << files.size() << '\n';
				for (auto i : files) {
					if (i->isFolder()) 
						cout << "/" << i->getFilename() << '\n';
					else 
						cout << i->getFilename() << "     "<<i->computeSize()<< "byte" << '\n';
				}

			}
			else {
				cout << "目标目录不存在\n";
			}
		}


	}
	else {
		auto folder = ConsoleApp::getInstance()->getFolderByPath(out);
		if (folder) {
			folder->show();
		}
		else {
			cout << "目标目录不存在\n";
		}
	}
}