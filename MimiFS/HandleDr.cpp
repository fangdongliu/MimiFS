#include "stdafx.h"
#include "HandleDr.h"

using namespace std;

REGISTER_HANDLER(HandleDr, "dr", "��ʾ�ļ���Ŀ¼", "dr [-b] [\"path\"]", "[-b]:��ѡ����������ģ��ƥ��\npath:��ѡ���������·�����߾���·��")


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
				cout << "ƥ���ļ�����" << files.size() << '\n';
				for (auto i : files) {
					if (i->isFolder()) 
						cout << "/" << i->getFilename() << '\n';
					else 
						cout << i->getFilename() << "     "<<i->computeSize()<< "byte" << '\n';
				}

			}
			else {
				cout << "Ŀ��Ŀ¼������\n";
			}
		}


	}
	else {
		auto folder = ConsoleApp::getInstance()->getFolderByPath(out);
		if (folder) {
			folder->show();
		}
		else {
			cout << "Ŀ��Ŀ¼������\n";
		}
	}
}