#include "stdafx.h"
#include "HandleDr.h"

REGISTER_HANDLER(HandleDr, "dr", "��ʾ�ļ���Ŀ¼", "dr [-b] [\"path\"]\n[-b]:��ѡ����������ģ��ƥ��\npath:��ѡ���������·�����߾���·��")


void HandleDr::onHandleCommand(Lexer&param) {
	
	using namespace std;
	
	bool blur = false;
	int count = 0;

	if (param.nextToken() == Lexer::Token::Command) {
		if (param.str == "b") {
			blur = true;
		}
		param.nextToken();
	}
		
	param.str = "";
	switch (param.token)
	{
	case Lexer::Token::RealString:
	case Lexer::Token::String:
		if (param.nextToken() != Lexer::Token::None)
			goto handleDrErr;
	case Lexer::Token::None: {

		vector<string>out;
		Helper::cutPathFromString(param.str, out);
		if (blur) {
			string filename;
			if (out.size()) {
				filename = out.back();
			}
			auto folder = ConsoleApp::getInstance()->getFolderByPath(out);
			
			if (folder) {

				std::vector<MiniFile*>files;
				folder->findMatchFiles(filename, files);
				count += files.size();
				for (auto i : files) {
					if (i->isFolder()) {
						cout << "/" << i->getFilename() << endl;
					}
					else {
						cout << i->getFilename() << "     "  << "byte" << endl;
					}
				}

			}
			else {
				cout << "Ŀ��Ŀ¼������\n";
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