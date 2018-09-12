#include "stdafx.h"
#include "HandleExport.h"
#include <filesystem>
#include"MiniFileReader.h"
using namespace std;
using namespace experimental::filesystem;

REGISTER_HANDLER(HandleExport, "export", "将文件(夹)导出到外部", "export \"filepath\" \"winFilepath\" \"filename\"", "filepath:要导出MiniFS文件路径\nwinFilepath:外部路径\nfilename:导出后的文件(夹)名")


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

	param >= from >= to >= filename >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	bool valid = false;

	if (!Helper::isRightFilename(filename))
		throw InvalidFilename();

	std::vector<std::string>pathlist;
	Helper::cutPathFromString(from, pathlist);
	if (!pathlist.size()) {
		cout << "目标文件不存在\n";
		return;
	}
	std::string filename2 = pathlist.back();
	pathlist.pop_back();

	auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
	if (!folder) {
		cout << "目标文件不存在\n";
		return;
	}
	try {
		MiniFile* b = nullptr;
		if (filename2 == ".") {
			b = folder;
		}
		else if (filename2 == "..") {
			if (folder->getParent() == nullptr) {
				cout << "目标文件不存在";
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
			cout << "外部存在同名文件(夹)" << filename << "，是否删除之?[Y/N] ";

			string str;
			getline(cin, str);
			if (str.length() == 1 && (str[0] == 'Y' || str[0] == 'y')) {
				remove_all(w2);
			}
			else {
				cout << "导出失败\n";
				return;
			}

		}

		string relativePath = "\\";
		string oldname = b->getFilename();
		b->setFilename(filename);
		doExport(relativePath, b, w);
		b->setFilename(oldname);
		cout << "导出文件夹数：" << cnt1 << '\n';
		cout << "导出文件数：" << cnt2 << '\n';
	}
	catch (exception&e) {
		cout << "目标文件不存在\n";
		return;
	}



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
		FILE *out = nullptr;
		if (!fopen_s(&out, (winPath.string() + relativePath + f->getFilename()).c_str(), "wb")) {
			MiniFileReader reader(f);
			char *buf = new char[reader.getBlockMaxReadSize() + 2];

			int n = 0;
			while (n = reader.readABlock(buf)) {
				fwrite(buf, 1, n, out);
			}
			fclose(out);
			cnt2++;
		}
	}

}