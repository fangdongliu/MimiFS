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
	string filename = "*";
	MiniFolder*folder = nullptr;


	if (blur) {
		if (out.size()) {
			filename = out.back();
			out.pop_back();
			folder = ConsoleApp::getInstance()->getFolderByPath(out);
		}
	}
	else {
		folder = ConsoleApp::getInstance()->getFolderByPath(out);
	}

	if (folder) {
		std::vector<MiniFile*>files;
		string output;
		folder->findMatchFiles(filename, files);
		cout << "文件数：" << files.size() << '\n';

		std::sort(files.begin(), files.end(), [](MiniFile* a, MiniFile* b)->bool {
			
			if (a->isFolder()) {
				if (!b->isFolder())
					return 0;
			}
			else if (b->isFolder())
				return 1;
			return a->getFilename() > b->getFilename();
		});

		for (auto i : files) {
			if (i->isFolder()) {
				if (i->getFilename().length() < 20) {
					printf("%- 20s", i->getFilename().c_str());
					
				}
				else if(i->getFilename().length() < 40){
					printf("%- 40s", i->getFilename().c_str());
				}
				else {
					printf("%s  ", i->getFilename().c_str());
				}
				printf("文件夹\n");
			}
			else {
				if (i->getFilename().length() < 20) {
					printf("%- 20s", i->getFilename().c_str());
				}
				else if (i->getFilename().length() < 40) {
					printf("%- 40s", i->getFilename().c_str());
				}
				else {
					printf("%s  ", i->getFilename().c_str());
				}

				int size = i->computeSize();
				if (size > (1 << 20)) {
					printf("大小：%.2lf MB\n",size / 1024.0 / 1024.0);
				}
				else if (size > 1024) {
					printf("大小：%.2lf KB\n",size / 1024.0);
				}
				else {
					printf("大小：%d B\n",size);
				}
			}
		
		}
	}
	else {
		cout << "目标目录不存在\n";
	}
}
