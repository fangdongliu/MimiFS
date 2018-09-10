#include "stdafx.h"
#include "HandleRead.h"

#include "MiniFileReader.h"

REGISTER_HANDLER(HandleRead,"read","��ȡһ���ļ�������","read \"filepath\"\nfilepath:Ҫ��ȡ�ļ�������")

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
			cout << "Ŀ���ļ�������\n";
		}

		return;
	}


}
