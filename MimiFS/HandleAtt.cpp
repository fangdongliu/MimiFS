#include "stdafx.h"
#include "HandleAtt.h"

using namespace std;

REGISTER_HANDLER(HandleAtt, "att", "��ʾ�ļ�(��)����", "att [-b] \"filepath\"", "[-b]:����ģ��ƥ��\nfilepath:Ŀ��·��")

HandleAtt::HandleAtt()
{
}


HandleAtt::~HandleAtt()
{

}

void HandleAtt::onHandleCommand(Lexer&param) {

	string filepath;

	param >= filepath >= Lexer::end;

	if (!param.matchSuccess())
		throw CommandFormatError();

	bool blur = param.findSwitch("b");


	vector<string>out;
	Helper::cutPathFromString(filepath, out);
	if (blur) {
		string filename;

		if (out.size()) {
			filename = out.back();
			out.pop_back();

			auto folder = ConsoleApp::getInstance()->getFolderByPath(out);

			if (folder) {
				std::vector<MiniFile*>files;
				folder->findMatchFiles(filename, files);
				cout << "ƥ���ļ�����" << files.size() << '\n';
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
		try{
		auto file = ConsoleApp::getInstance()->getFileByPath(out);
			if (file) {
					file->showAtt();
			}
		}
		catch (const PathNotExist&) {
			cout << "Ŀ��·��������\n";
		}
		catch (const exception&) {
			cout << "δ�ҵ�Ŀ���ļ�\n";
		}


	}
	
}