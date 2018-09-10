#include "stdafx.h"
#include "HandleExport.h"
#include <filesystem>
#include"MiniFileReader.h"
using namespace std;
using namespace experimental::filesystem;

REGISTER_HANDLER(HandleExport,"export","���ļ�(��)�������ⲿ","export \"filepath\" \"winFilepath\" \"filename\"\nfilepath:Ҫ����MiniFS�ļ�·��\nwinFilepath:�ⲿ·��\nfilename:��������ļ�(��)��")


HandleExport::HandleExport()
{
}


HandleExport::~HandleExport()
{
}

void HandleExport::onHandleCommand(Lexer&param) {

	cnt1 = 0;
	cnt2 = 0;
	string from, to;
	string filename;
	if (!param.nextTokenMatchString()) {
		goto exportError;
	}
	from = param.str;

	if (!param.nextTokenMatchString()) {
		goto exportError;
	}
	to = param.str;

	if (!param.nextTokenMatchString()) {
		goto exportError;
	}
	filename = param.str;
	if (!param.nextTokenMatchEnd())
		goto exportError;

	{
		bool valid = false;

		for (auto i : filename) {
			if (!(i == ' ' || i == '.' || i == '\\' || i == '/' || i == '\t')) {
				valid = true;
				break;
			}
		}

		if (!valid) {
			cout << "���Ϸ����ⲿ�ļ���\n";
			return;
		}

		

		{
			std::vector<std::string>pathlist;
			Helper::cutPathFromString(from, pathlist);
			if (!pathlist.size()) {
				cout << "Ŀ���ļ�������\n";
				return;
			}
			std::string filename2 = pathlist.back();
			pathlist.pop_back();

			auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
			if (!folder) {
				cout << "Ŀ���ļ�������\n";
				return;
			}
			try {
				MiniFile* b=nullptr;
				if (filename2 == ".") {
					b = folder;
				}
				else if (filename2 == "..") {
					if (folder->getParent() == nullptr) {
						cout << "Ŀ���ļ�������";
						return;
					}
					else {
						b = folder->getParent();
					}
				}
				else {
					b = folder->atChild(filename2);
				}

				

				if (!b) {
					throw exception();
				}

				path w(to);
				path w2(to + "\\" + filename);
				if (is_directory(w2) || is_regular_file(w2)) {
					cout << "�ⲿ����ͬ���ļ�(��)" << filename << "���Ƿ�ɾ��֮?[Y/N] ";

					string str;
					getline(cin, str);
					if (str.length() == 1 && (str[0] == 'Y' || str[0] == 'y')) {
						remove_all(w);
					}
					else {
						cout << "����ʧ��\n";
						return;
					}

				}

				string relativePath = "\\";
				string oldname = b->getFilename();
				b->setFilename(filename);
				doExport(relativePath, b, w);
				b->setFilename(oldname);
				cout << "�����ļ�������" << cnt1 << endl;
				cout << "�����ļ�����" << cnt2 << endl;
			}
			catch (exception&e) {
				cout << "Ŀ���ļ�������\n";
				return;
			}


			return;
		}
	}
exportError:
	cout << "export \"filepath\" \"winFilepath\" \"filename\"";
}

void HandleExport::doExport(std::string& relativePath, MiniFile*f, path&winPath) {

	if (f->isFolder()) {
		if (create_directory((winPath.string() + relativePath + f->getFilename()))) {
			cnt1++;

			auto folder = (MiniFolder*)f;
			if (folder->isLoaded())
				folder->load();
			vector<MiniFile*>files;
			string pattern = "*";
			folder->findMatchFiles(pattern, files);

			auto relativeNew = relativePath + f->getFilename() + "/";

			for (auto i : files) {
				doExport(relativeNew, i, winPath);
			}
		}
	}
	else {
		FILE *out=nullptr;
		if (!fopen_s(&out, (winPath.string() + relativePath + f->getFilename()).c_str(), "wb")) {
			MiniFileReader reader(f);
			char *buf=new char[reader.getBlockMaxReadSize()+2];
			
			int n=0;
			while (n=reader.readABlock(buf)) {
				fwrite(buf, 1, n, out);
			}
			fclose(out);
			cnt2++;
		}
	}

}