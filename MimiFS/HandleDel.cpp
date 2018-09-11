#include "stdafx.h"
#include "HandleDel.h"


REGISTER_HANDLER(HandleDel, "del", "ɾ��ָ���ļ�(��)", "del [-b] \"filepath\"", "[-b]��ѡ����:����ģ��ƥ��\nfilepath:Ŀ���ļ�(��)·��")

void HandleDel::onHandleCommand(Lexer&param) {

	using namespace std;

	string filepath;

	param >= filepath >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	bool blur = param.findSwitch("b");

	int count = 0;

	std::vector<std::string>pathlist;
	Helper::cutPathFromString(filepath, pathlist);

	if (pathlist.size()) {
		std::string filename = pathlist.back();
		pathlist.pop_back();
		auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist);
		if (!folder) {
			std::cout << "Ŀ��·��������\n";
		}
		if (blur) {
			std::vector<MiniFile*>files;
			folder->findMatchFiles(filename, files);

			if (files.size()) {
				count += files.size();
				for (auto i : files) {
					string n = i->getFilename();
					count += i->deleteForever();
					delete i;
					folder->atChild(n) = nullptr;
				}

				folder->updateDir();
			}
		}
		else {
			try {
				auto &file = folder->atChild(filename);
				if (!file)
					throw exception();
				count++;
				count += file->deleteForever();
				delete file;
				file = nullptr;
				folder->updateDir();
			}
			catch (exception&e) {
				cout << "δ�ҵ�Ŀ���ļ�\n";

			}

		}
	}

	std::cout << "ɾ�����ļ�(��)����" << count << '\n';

}