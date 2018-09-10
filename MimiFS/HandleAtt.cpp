#include "stdafx.h"
#include "HandleAtt.h"

REGISTER_HANDLER(HandleAtt,"att","��ʾ�ļ�(��)����","att [-b] \"filepath\"\n[-b]:����ģ��ƥ��\nfilepath:Ŀ��·��")

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
					cout << "ƥ���ļ�����" << files.size() << endl;
					for (auto i : files) {
						i->showAtt();
					}

				}
				else {
					cout << "Ŀ��Ŀ¼������\n";
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
						cout << "Ŀ���ļ�������\n";
					}
				}
				else {
					cout << "Ŀ��Ŀ¼������\n";
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