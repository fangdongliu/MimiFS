#include "stdafx.h"
#include "HandleDr.h"

REGISTER_HANDLER(HandleDr, "dr", "��ʾ��ǰ�ļ���Ŀ¼", "dr [\"path\"]\npath:��ѡ���������·�����߾���·��")


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
		cout << "Ŀ��Ŀ¼������\n";
	}

	

}