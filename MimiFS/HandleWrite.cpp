#include "stdafx.h"
#include "HandleWrite.h"

#include"MiniFileWriter.h"

REGISTER_HANDLER(HandleWrite,"write","向一个文件中写入字符串","write \"filepath\" \"string\"\nfilepath:目标文件路径\nstring:将要写入的字符串")

HandleWrite::HandleWrite()
{
}


HandleWrite::~HandleWrite()
{
}

void HandleWrite::onHandleCommand(Lexer&param) {

	using namespace std;

	string from, to;
	if (!param.nextTokenMatchString()) {
		goto writeError;
	}
	from = param.str;

	if (!param.nextTokenMatchString()) {
		goto writeError;
	}
	to = param.str;

	if (!param.nextTokenMatchEnd())
		goto writeError;

	{
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

			int count = to.length()/maxlen;

			auto data = to.c_str();

			for (int i = 0; i < count; i++) {
				writer.writeToBlock(data + i * maxlen, maxlen);
			}

			int remain = to.length() - count * maxlen;
			if (remain > 0) {
				writer.writeToBlock(data + count * maxlen, remain);
			}

			folder->updateDir();

		}
		catch (exception&e) {
			cout << "目标文件不存在\n";
		}

		return;
	}

writeError:
	cout << "write \"filepath\" \"string\"\n";

}
