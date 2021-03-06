// MimiFS.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ConsoleApp.h"

using namespace std;

int main()
{
	auto app = ConsoleApp::getInstance();

	string cmd;

	cout << "\n";
	cout << "welcome to mimiFS!\n";
	cout << "version 1.0\n";
	cout << "输入 \"help\" 查看帮助\n\n";

	app->printPrefix();

	while (getline(cin, cmd)) {

		if (cmd == "exit")
			break;
		else if (cmd == "clear") {
			system("cls");
		}
		else if(cmd!=""){
			Lexer lexer(cmd);
			if (lexer.getName() == "") {
				cout << "wrong command\n";
			}
			else {
				auto t = timeGetTime();
				app->handleCommand(lexer);

				cout << "操作用时:" << timeGetTime() - t << "ms\n";
				cout << '\n';
			}
		}
		app->printPrefix();
	}

	return 0;
}

