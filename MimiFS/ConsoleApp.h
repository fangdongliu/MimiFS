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
	static ConsoleApp * getInstance();//获取App单例

	void handleCommand(Lexer&);			//处理命令
	void addHandler(CommandHandler*);	//添加处理器
	void printPrefix();					//显示控制台前缀
	
	int getBlockSize();

	MiniFile* getFileByPath(std::vector<std::string>&);
	MiniFolder* getFolderByPath(std::vector<std::string>&,bool queryCreateNew=true);//根据路径获取对应文件夹

	std::unordered_map<std::string, CommandHandler*>handler;
	std::unordered_map<std::string, HelpItem>helpList;

	RootFolder root;
	//TrashBin trashBin;
	MiniFolder *current;//当前目录

protected:
	void optFS(Lexer&);
	void closeMiniFS(Lexer&);			 //关闭系统
	void formatMiniFS(Lexer&);			 //格式化系统
	void showHelp(Lexer&);				 //显示帮助
	void createMiniFS(Lexer&);			 //创建MiniFS空间
	void mountMiniFS(Lexer&);			 //装载MiniFS空间
	void showFSInfo(Lexer&);			 //显示系统属性
	bool ready();

	bool doOpt(std::vector<int>emptys,MiniFile*);
};