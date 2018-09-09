#include "stdafx.h"
#include "HandleDr.h"

REGISTER_HANDLER(HandleDr, "dr", "��ʾ�ļ���Ŀ¼", "dr [-b] [\"path\"]\n[-b]:��ѡ����������ģ��ƥ��\npath:��ѡ���������·�����߾���·��")


void HandleDr::onHandleCommand(Lexer&param) {
	
	using namespace std;
	
	bool blur = false;
	param.str = "";
	if (param.nextToken() == Lexer::Token::Command) {
		if (param.str == "b") {
			blur = true;
		}
		param.str = "";
		param.nextToken();
	}
	string pathname = param.str;
	param.str = "";
	switch (param.token)
	{
	case Lexer::Token::RealString:
	case Lexer::Token::String:
		if (param.nextToken() != Lexer::Token::None)
			goto handleDrErr;
	case Lexer::Token::None: {

		vector<string>out;
		Helper::cutPathFromString(pathname, out);
		if (blur) {
			string filename;
			
			if (out.size()) {
				filename = out.back();
				out.pop_back();

				auto folder = ConsoleApp::getInstance()->getFolderByPath(out);

				if (folder) {
					std::vector<MiniFile*>files;
					folder->findMatchFiles(filename, files);
					cout << "ƥ���ļ�����" << files.size() << endl;
					for (auto i : files) {
						if (i->isFolder()) {
							cout << "/" << i->getFilename() << endl;
						}
						else {
							cout << i->getFilename() << "     " << "byte" << endl;
						}
					}
					
				}
				else {
					cout << "Ŀ��Ŀ¼������\n";
				}
			}
			

		}
		else {
			auto folder = ConsoleApp::getInstance()->getFolderByPath(out);
			if (folder) {
				folder->show();
			}
			else {
				cout << "Ŀ��Ŀ¼������\n";
			}
		}
		return;
	}
	default:
		goto handleDrErr;
	}


handleDrErr:
	cout << "dr [-b] [\"path\"]";

}