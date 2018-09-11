#include "stdafx.h"
#include "HandleImport.h"
#include "MiniFileWriter.h"
using namespace std;
using namespace experimental::filesystem;

REGISTER_HANDLER(HandleImport, "import", "���ⲿ�ļ�(��)����ϵͳ", "import \"winFilepath\" \"filepath\"", "winFilepath:�ⲿ·��\nfilepath:MiniFS�ļ�·��")

HandleImport::HandleImport()
{
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
	if (is_directory(w)) {
		cnt1++;
		doImport(folder->createChildFolder(filename), w);
		folder->updateDir();
	}
	else if (is_regular_file(w)) {

		FILE*file;

		if (!fopen_s(&file, w.string().c_str(), "rb")) {
			auto child = folder->createChildFile(filename);

			MiniFileWriter writer(child);

			int maxsize = writer.getBlockMaxWriteSize();

			char *buf = new char[maxsize + 1];
			int len = 0;
		readABlock:
			len = fread(buf, 1, maxsize, file);
			if (len) {
				writer.writeToBlock(buf, len);
				goto readABlock;
			}
			fclose(file);
			folder->updateDir();
			cnt2++;
		}
	}
	cout << "�����ļ�������" << cnt1 << '\n';
	cout << "�����ļ�����" << cnt2 << '\n';


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
		string name = winPath.filename().string();
		auto child = f->createChildFile(name);

		MiniFileWriter writer(child);

		int maxsize = writer.getBlockMaxWriteSize();

		char *buf = new char[maxsize + 1];
		int len = 0;
	readABlock:
		len = fread(buf, 1, maxsize, file);
		if (len) {
			writer.writeToBlock(buf, len);
			goto readABlock;
		}
		fclose(file);
		cnt2++;
	}

}