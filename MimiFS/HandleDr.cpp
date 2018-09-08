#include "stdafx.h"
#include "HandleDr.h"

REGISTER_HANDLER(HandleDr, "dr", "显示当前文件夹目录", "dr [\"path\"]\npath:可选参数，相对路径或者绝对路径")


void HandleDr::onHandleCommand(std::istream&param) {
	
	using namespace std;

	

	string pathname;
	
	param >> pathname;
	/*getline(param, pathname);

	int i = 0;
	for (; i < pathname.length(); i++) {
		if (pathname[i] != ' ' && pathname[i] != '\t')
			break;
	}

	int start = pathname.find_first_of('"',i);

	if (start == pathname.npos) {
		sscanf("%s", pathname.substr(start + 1).c_str(),buf);
		return;
	}


	int end = pathname.find_last_of('"');

	if (start == end) {
		pathname = pathname.substr(start + 1);
	}
	else {
		pathname = pathname.substr(start + 1, end - start);
	}*/
	
	vector<string>out;

	Helper::cutPathFromString(pathname, out);

	auto f=ConsoleApp::getInstance()->getFolderByPath(out);

	if (f) {
		f->show();
	}
	else {
		cout << "目标目录不存在\n";
	}

	

}