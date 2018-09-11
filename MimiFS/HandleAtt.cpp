#include "stdafx.h"
#include "HandleAtt.h"

using namespace std;

REGISTER_HANDLER(HandleAtt, "att", "显示文件(夹)属性", "att [-b] \"filepath\"", "[-b]:开启模糊匹配\nfilepath:目标路径")

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
				cout << "匹配文件数：" << files.size() << '\n';
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
		try{
		auto file = ConsoleApp::getInstance()->getFileByPath(out);
			if (file) {
					file->showAtt();
			}
		}
		catch (const PathNotExist&) {
			cout << "目标路径不存在\n";
		}
		catch (const exception&) {
			cout << "未找到目标文件\n";
		}


	}
	
}