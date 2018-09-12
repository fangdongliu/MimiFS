#include "stdafx.h"
#include "HandleImport.h"
#include "MiniFileWriter.h"
using namespace std;
using namespace experimental::filesystem;

REGISTER_HANDLER(HandleImport, "import", "���ⲿ�ļ�(��)����ϵͳ", "import \"winFilepath\" \"filepath\" \"filename\"", "winFilepath:�ⲿ·��\nfilepath:MiniFS�ļ�·��")

HandleImport::HandleImport()
{
	importBuf = 0;
}


HandleImport::~HandleImport()
{
}

void HandleImport::onHandleCommand(Lexer&param) {

	cnt1 = 0;
	cnt2 = 0;
	string from, to, filename;

	

	param >= from >= to >= filename >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();


	path w(from);
	if (!is_directory(w) && !is_regular_file(w)) {
		cout << "�ⲿ�ļ�(" << w << ")������\n";
		return;

	}
	std::vector<std::string>pathlist;
	Helper::cutPathFromString(to, pathlist);

	auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);

	if (!folder) {
		cout << "Ŀ��·��������\n";
		return;
	}

	auto&b = folder->getChild(filename);
	if (b) {
		cout << "Ŀ���ļ��Ѵ��ڣ��Ƿ񸲸�?[Y/N]:";
		string str;
		getline(cin, str);
		if (str.length() == 1 && str[0] == 'y' || str[0] == 'Y') {
			b->deleteForever();
			delete b;
			b = nullptr;
		}
		else {
			cout << "�޸�ʧ��";
			return;
		}
	}

	if (importBuf)
		delete[]importBuf;

	importBuf = new char[100000];

	if (is_directory(w)) {
		cnt1++;
		doImport(folder->createChildFolder(filename), w);
		folder->updateDir();
	}
	else if (is_regular_file(w)) {
		FILE*file;
		if (!fopen_s(&file, w.string().c_str(), "rb")) {
			int filesize = getWinFileSize(file);

			fseek(file, 0, SEEK_SET);

			int blockCount = filesize / ConsoleApp::getInstance()->getBlockSize() + 1;

			auto child = folder->createChildFile(filename, blockCount);

			MiniFileWriter writer(child);

			if (filesize < 100000) {
				fread(importBuf, 1, filesize, file);
				int cur = 0;

				while (filesize) {
					int n = writer.queryCurrentMaxWriteSize();

					if (filesize > n) {
						writer.writeToBlock(importBuf + cur, n);
						filesize -= n;
						cur += n;
					}
					else {
						writer.writeToBlock(importBuf + cur, filesize);
						filesize = 0;
					}
				}
			}
			else {
				while (1) {
					int n = writer.queryCurrentMaxWriteSize();
					int len = fread(importBuf, 1, n, file);

					if (len == 0)
						break;

					writer.writeToBlock(importBuf, len);

				}
			}

			fclose(file);
			folder->updateDir();
			cnt2++;
			printf("�����ļ�:%s\n", w.string().c_str());
		}
	}
	cout << "�����ļ�������" << cnt1 << '\n';
	cout << "�����ļ�����" << cnt2 << '\n';

	delete[]importBuf;
	importBuf = nullptr;
}

void HandleImport::doImport(MiniFolder*f, const std::experimental::filesystem::path& winPath) {

	int count = 0;

	for (auto& i : directory_iterator(winPath)) {
		if (is_directory(i)) {
			cnt1++;
			string filename = i.path().filename().string();
			doImport(f->createChildFolder(filename), i.path());
			count++;
		}
		else if (is_regular_file(i)) {
			importFile(f, i.path());
			count++;
		}
	}
	if (count)
		f->updateDir();

}

void HandleImport::importFile(MiniFolder*f, const std::experimental::filesystem::path &winPath) {

	FILE*file;

	if (!fopen_s(&file, winPath.string().c_str(), "rb")) {
		printf("���ڵ����ļ�:%s\n", winPath.string().c_str());
		int filesize = getWinFileSize(file);

		fseek(file, 0, SEEK_SET);

		string name = winPath.filename().string();
		int blockCount = filesize / ConsoleApp::getInstance()->getBlockSize() + 1;

		auto child = f->createChildFile(name, blockCount);

		MiniFileWriter writer(child);

		if (filesize < 100000) {
			fread(importBuf, 1, filesize, file);
			int cur = 0;

			while (filesize) {
				int n = writer.queryCurrentMaxWriteSize();

				if (filesize > n) {
					writer.writeToBlock(importBuf + cur, n);
					filesize -= n;
					cur += n;
				}
				else {
					writer.writeToBlock(importBuf + cur, filesize);
					filesize = 0;
				}
			}
		}
		else{
			while (1) {
				int n = writer.queryCurrentMaxWriteSize();
				int len = fread(importBuf, 1, n, file);

				if (len == 0)
					break;

				writer.writeToBlock(importBuf, len);

			}
		}

		
		fclose(file);
		cnt2++;
		
	}

}

int HandleImport::getWinFileSize(FILE*file) {

	fseek(file, 0, SEEK_END);

	return ftell(file);

}