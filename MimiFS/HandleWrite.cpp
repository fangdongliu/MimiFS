#include "stdafx.h"
#include "HandleWrite.h"

#include"MiniFileWriter.h"

REGISTER_HANDLER(HandleWrite, "write", "��һ���ļ���д���ַ���", "write \"filepath\" \"string\"", "filepath:Ŀ���ļ�·��\nstring:��Ҫд����ַ���")

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
		cout << "Ŀ���ļ�������\n";
		return;
	}
	std::string filename = pathlist.back();
	pathlist.pop_back();

	auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);
	if (!folder) {
		cout << "Ŀ���ļ�������\n";
		return;
	}

	try {
		auto& b = folder->atChild(filename);

		if (!b) {
			throw exception();
		}
		if (b->isFolder()) {
			cout << "Ŀ�����ļ���\n";
			return;
		}

		MiniFileWriter writer(b);


		char* utf8 = new char[to.length() * 2];

		wchar_t*unicode = new wchar_t[to.length() * 2];

		Helper::ASCIIToUTF8(to.c_str(), utf8);

		Helper::UTF8ToUnicode(utf8, unicode);

		wstring wstr = unicode;

		auto data = wstr.c_str();

		int remain = wstr.length() * 2;

		int cur=0;

		while (remain > 0) {

			int n = writer.queryCurrentMaxWriteSize();

			if (remain > n) {
				remain -= n;
				writer.writeToBlock(((const char*)(data)) + cur, n);
			}
			else {
				writer.writeToBlock(((const char*)(data)) + cur, remain);
				remain = 0;
			}
			
			cur += n;

		}
		folder->updateDir();

	}
	catch (exception&e) {
		cout << "Ŀ���ļ�������\n";
	}

}
