#include "stdafx.h"
#include "HandleDel.h"

REGISTER_HANDLER(HandleDel, "del", "删除指定文件(夹)", "del [-b] \"filename\" ...\n[-b]可选参数:开启模糊匹配")



void HandleDel::onHandleCommand(Lexer&param) {

	using namespace std;

	bool blur = false;
	int count = 0;

	if (param.nextToken() == Lexer::Token::Command) {
		if (param.str == "b") {
			blur = true;
		}
		param.nextToken();
	}


	if (param.token == Lexer::Token::None) {
		goto delError;
	}

	while (1) {
		switch (param.token) {
		case Lexer::Token::None:
			cout << "删除的文件数:" << count << endl;
			return;
		case Lexer::Token::RealString:
		case Lexer::Token::String: {
			std::vector<std::string>pathlist;
			Helper::cutPathFromString(param.str, pathlist);
			if (pathlist.size()) {
				std::string filename = pathlist.back();
				pathlist.pop_back();

				auto folder = ConsoleApp::getInstance()->getFolderByPath(pathlist, false);

				if (folder) {
					if (blur) {
						std::vector<MiniFile*>files;
						folder->findMatchFiles(filename, files);
						if (files.size()) {
							count += files.size();
							for (auto i : files) {
								string n = i->getFilename();
								i->deleteForever();
								delete i;
								folder->atChild(n) = nullptr;
							}

							folder->updateDir();
						}
					}
					else {
						try {
							auto& b = folder->atChild(filename);
							if (b) {
								b->deleteForever();
								delete b;
								b = nullptr;
								count++;
								folder->updateDir();
							}
						}
						catch (exception&e) {
							
						}
					}
				}
			}
			break;
		}
		default:
			goto delError;
		}
		param.nextToken();
	}

delError:
	cout << "del [-b] \"filename\" ... \n";
	return;
}