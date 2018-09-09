#include "stdafx.h"
#include "HandleMd.h"

REGISTER_HANDLER(HandleMd, "md", "�½�һ���ļ���", "md \"path\" \npath:��ѡ���������·�����߾���·��")

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
				cout << "����ʧ��\n";
			}

			auto child = f->getChild(filename);
			if (child) {
				cout << "Ŀ���ļ��Ѵ���\n";
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
