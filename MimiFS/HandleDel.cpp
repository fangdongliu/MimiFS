#include "stdafx.h"
#include "HandleDel.h"

REGISTER_HANDLER(HandleDel,"del","ɾ��һ���ļ����ļ���","del \"filename\"")



void HandleDel::onHandleCommand(std::istream&param) {

	using namespace std;

	string name;
	param >> name;

	auto i = ConsoleApp::getInstance()->current;

	try {
		auto& b = i->atChild(name);
		if (b) {
			b->deleteForever();
			delete b;
			b = nullptr;
			i->updateDir();
		}
		else {
			cout << "Ŀ���ļ�: " + name + " ������" << endl;
		}
	}
	catch (exception&e) {
		cout << "Ŀ���ļ�: " + name + " ������" << endl;
	}
}