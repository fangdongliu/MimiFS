#pragma once

#include "RootFolder.h"
#include "TrashBin.h"

class ConsoleApp SINGLE_INSTANCE
{
	ConsoleApp();
	static ConsoleApp* instance;
	ConsoleApp(const ConsoleApp& src) {}
	ConsoleApp &operator=(const ConsoleApp& src) {};

public:
	static ConsoleApp * getInstance() {
		if (!instance) {
			instance = new ConsoleApp;
		}
		return instance;
	}

	void handleCommand(std::string&);

	void addHandler(CommandHandler*);

	void showFSInfo(Lexer&);
	void showHelp(Lexer&);
	void createMiniFS(Lexer&);
	void mountMiniFS(Lexer&);

	void closeMiniFS(Lexer&);
	void formatMiniFS(Lexer&);

	void printPrefix();

	MiniFolder* getFolderByPath(std::vector<std::string>&,bool queryCreateNew=true);

	std::unordered_map<std::string, CommandHandler*>handler;
	std::unordered_map<std::string, HelpItem>helpList;

	RootFolder root;
	TrashBin trashBin;
	MiniFolder *current;

};