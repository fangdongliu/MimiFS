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
		cout << "�ļ�����" << files.size() << '\n';

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
				printf("�ļ���\n");
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
					printf("��С��%.2lf MB\n",size / 1024.0 / 1024.0);
				}
				else if (size > 1024) {
					printf("��С��%.2lf KB\n",size / 1024.0);
				}
				else {
					printf("��С��%d B\n",size);
				}
			}
		
		}
	}
	else {
		cout << "Ŀ��Ŀ¼������\n";
	}
}
