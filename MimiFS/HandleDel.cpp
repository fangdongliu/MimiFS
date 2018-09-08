#include "stdafx.h"
#include "HandleDel.h"

REGISTER_HANDLER(HandleDel,"del","删除一个文件或文件夹","del \"filename\"")



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
			cout << "目标文件: " + name + " 不存在" << endl;
		}
	}
	catch (exception&e) {
		cout << "目标文件: " + name + " 不存在" << endl;
	}
}