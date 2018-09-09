#include "stdafx.h"
#include "HandleMd.h"

REGISTER_HANDLER(HandleMd, "md", "新建一个文件夹", "md \"path\" \npath:可选参数，相对路径或者绝对路径")

void HandleMd::onHandleCommand(Lexer&param) {

	using namespace std;

	if (param.nextToken() == Lexer::Token::None) {
		goto mdError;
	}
	switch (param.token)
	{
	case Lexer::Token::RealString:
	case Lexer::Token::String: {

		vector<string>out;

		Helper::cutPathFromString(param.str, out);
		if (out.size()) {
			string filename = out.back();

			out.pop_back();

			auto f = ConsoleApp::getInstance()->getFolderByPath(out);

			if (!f) {
				cout << "创建失败\n";
			}

			auto child = f->getChild(filename);
			if (child) {
				cout << "目标文件已存在\n";
			}
			else {
				f->createChildFolder(filename);
				f->updateDir();
			}
			return;
		}
	}
	default:
		goto mdError;
	}

mdError:
	cout << "del \"filename\" ... \n";
	return;


}
