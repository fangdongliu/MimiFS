#include "stdafx.h"
#include "HandleAtt.h"

REGISTER_HANDLER(HandleAtt,"att","显示文件(夹)属性","att [-b] \"filepath\"\n[-b]:开启模糊匹配\nfilepath:目标路径")

HandleAtt::HandleAtt()
{
}


HandleAtt::~HandleAtt()
{

}

void HandleAtt::onHandleCommand(Lexer&param) {

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
			goto AttErr;
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
					cout << "匹配文件数：" << files.size() << endl;
					for (auto i : files) {
						i->showAtt();
					}

				}
				else {
					cout << "目标目录不存在\n";
				}
			}


		}
		else {
			string filename;

			if (out.size()) {
				filename = out.back();
				out.pop_back();
				auto folder = ConsoleApp::getInstance()->getFolderByPath(out);
				if (folder) {
					try {
						auto b = folder->atChild(filename);
						b->showAtt();
					}
					catch(exception&e){
						cout << "目标文件不存在\n";
					}
				}
				else {
					cout << "目标目录不存在\n";
				}
			}
			
			
		}
		return;
	}
	default:
		goto AttErr;
	}


AttErr:
	cout << "att [-b] \"filepath\"";

}