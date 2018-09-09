#include "stdafx.h"
#include "HandleMf.h"

REGISTER_HANDLER(HandleMf,"mf","����һ���µ��ļ�","mf \"filename\"\nfilename:Ҫ�����ļ����ļ���")

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
				cout << "����ʧ��\n";
			}

			auto child = f->getChild(filename);
			if (child) {
				cout << "Ŀ���ļ��Ѵ���\n";
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