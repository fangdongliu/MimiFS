#include "stdafx.h"
#include "HandleRead.h"

#include "MiniFileReader.h"

REGISTER_HANDLER(HandleRead, "read", "读取一个文件的数据", "read \"filepath\"\nfilepath:要读取文件的数据")

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

			unsigned char *buf = new unsigned char[maxlen + 4];

			bool wMode = false;
			char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));

			while (int n = reader.readABlock((char*)buf)) {
				if (!wMode) {
					for (int i = 0; i < n; i++) {
						if (buf[i] > 127) {
							wMode = true;
							setlocale(LC_CTYPE, "");
							break;

						}
					}
				}
				if (wMode) {
					wchar_t *wBuf = (wchar_t *)buf;
					wBuf[(n+1) / 2] = 0;
					wprintf(L"%s",wBuf);
				}
				else {
					buf[n] = 0;
					cout << buf;
				}
			}
			if (wMode)
				setlocale(LC_CTYPE, old_locale);
			cout << '\n';
		}
		catch (exception&e) {
			cout << "目标文件不存在\n";
		}

		return;
	}


}
