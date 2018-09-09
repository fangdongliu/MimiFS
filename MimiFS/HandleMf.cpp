#include "stdafx.h"
#include "HandleMf.h"

REGISTER_HANDLER(HandleMf,"mf","创建一个新的文件","mf \"filename\"\nfilename:要创建文件的文件名")

void HandleMf::onHandleCommand(Lexer&param) {

	using namespace std;

	if (param.nextToken() == Lexer::Token::None) {
		goto mfError;
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
				f->createChildFile(filename);
				f->updateDir();
			}
			return;
		}
	}
	default:
		goto mfError;
	}

mfError:
	cout << "mf \"filename\"  \n";
	return;

}