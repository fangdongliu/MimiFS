#include "stdafx.h"
#include "HandleDr.h"

REGISTER_HANDLER(HandleDr, "dr", "显示文件夹目录", "dr [-b] [\"path\"]\n[-b]:可选参数，开启模糊匹配\npath:可选参数，相对路径或者绝对路径")


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
				cout << "目标目录不存在\n";
			}

		}
		else {
			auto folder = ConsoleApp::getInstance()->getFolderByPath(out);
			if (folder) {
				folder->show();
			}
			else {
				cout << "目标目录不存在\n";
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