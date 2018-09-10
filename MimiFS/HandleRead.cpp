#include "stdafx.h"
#include "HandleRead.h"

#include "MiniFileReader.h"

REGISTER_HANDLER(HandleRead,"read","读取一个文件的数据","read \"filepath\"\nfilepath:要读取文件的数据")

HandleRead::HandleRead()
{
}


HandleRead::~HandleRead()
{
}

void HandleRead::onHandleCommand(Lexer&param) {

	using namespace std;

	string pathname;

	if (param.nextTokenMatchString() && param.nextTokenMatchEnd()) {
		pathname = param.str;
	}
	else {
		cout << "read \"filepath\"\n";
		return;
	}

	{
		std::vector<std::string>pathlist;
		Helper::cutPathFromString(pathname, pathlist);
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

			MiniFileReader reader(b);

			int maxlen = reader.getBlockMaxReadSize();

			char *buf = new char[maxlen+1];

			while (int n = reader.readABlock(buf)) {
				buf[n] = 0;
				cout << buf;
			}
			cout << '\n';
		}
		catch (exception&e) {
			cout << "目标文件不存在\n";
		}

		return;
	}


}
