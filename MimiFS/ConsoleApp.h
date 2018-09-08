#pragma once

#include "RootFolder.h"
#include "TrashBin.h"

class ConsoleApp SINGLE_INSTANCE
{
	ConsoleApp();
	static ConsoleApp* instance;
	ConsoleApp(const ConsoleApp& src) {}
	ConsoleApp &operator=(const ConsoleApp& src) {};

	class Garbo   
	{
	public:
		~Garbo()
		{
			if (ConsoleApp::instance)
			{
				delete ConsoleApp::instance;
				ConsoleApp::instance = NULL;
			}
		}
	};
	static Garbo garbo;

public:
	static ConsoleApp * getInstance() {
		if (!instance) {
			instance = new ConsoleApp;
		}
		return instance;
	}

	void handleCommand(std::string&);

	void addHandler(CommandHandler*);

	void showHelp(std::istream&);
	void createMiniFS(std::istream& param);
	void mountMiniFS(std::istream& param);

	void closeMiniFS();
	void formatMiniFS(std::istream&);

	void printPrefix();

	MiniFolder* getFolderByPath(std::vector<std::string>&,bool queryCreateNew=true);

	std::unordered_map<std::string, CommandHandler*>handler;
	std::unordered_map<std::string, HelpItem>helpList;

	RootFolder root;
	TrashBin trashBin;
	MiniFolder *current;

};