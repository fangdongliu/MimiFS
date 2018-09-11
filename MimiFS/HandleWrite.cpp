#include "stdafx.h"
#include "HandleWrite.h"

#include"MiniFileWriter.h"

REGISTER_HANDLER(HandleWrite, "write", "向一个文件中写入字符串", "write \"filepath\" \"string\"", "filepath:目标文件路径\nstring:将要写入的字符串")

HandleWrite::HandleWrite()
{
}


HandleWrite::~HandleWrite()
{
}

void HandleWrite::onHandleCommand(Lexer&param) {

	using namespace std;

	string from, to;

	param >= from >= to >= Lexer::end;


	std::vector<std::string>pathlist;
	Helper::cutPathFromString(from, pathlist);
	if (!pathlist.size()) {
		cout << "目标文件不存在\n";
		return;
	}
	std::string filename = pathlist.back();
	pathlist.pop_back();

	auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
	if (!folder) {
		cout << "目标文件不存在\n";
		return;
	}

	try {
		auto& b = folder->atChild(filename);

		if (!b) {
			throw exception();
		}
		if (b->isFolder()) {
			cout << "目标是文件夹\n";
			return;
		}

		MiniFileWriter writer(b);

		int maxlen = writer.getBlockMaxWriteSize();



		char* utf8 = new char[to.length() * 2];

		wchar_t*unicode = new wchar_t[to.length() * 2];

		Helper::ASCIIToUTF8(to.c_str(), utf8);

		Helper::UTF8ToUnicode(utf8, unicode);

		wstring wstr = unicode;


		auto data = wstr.c_str();

		int count = wstr.length() * 2 / maxlen;

		for (int i = 0; i < count; i++) {
			writer.writeToBlock((const char*)(data + i * maxlen), maxlen);
		}

		int remain = wstr.length() * 2 - count * maxlen;
		if (remain > 0) {
			writer.writeToBlock((const char*)(data + count * maxlen), remain);
		}

		folder->updateDir();

	}
	catch (exception&e) {
		cout << "目标文件不存在\n";
	}

}
