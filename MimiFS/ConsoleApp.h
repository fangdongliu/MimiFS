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
	static ConsoleApp * getInstance();//��ȡApp����

	void handleCommand(Lexer&);			//��������
	void addHandler(CommandHandler*);	//��Ӵ�����
	void printPrefix();					//��ʾ����̨ǰ׺
	
	int getBlockSize();

	MiniFile* getFileByPath(std::vector<std::string>&);
	MiniFolder* getFolderByPath(std::vector<std::string>&,bool queryCreateNew=true);//����·����ȡ��Ӧ�ļ���

	std::unordered_map<std::string, CommandHandler*>handler;
	std::unordered_map<std::string, HelpItem>helpList;

	RootFolder root;
	//TrashBin trashBin;
	MiniFolder *current;//��ǰĿ¼

protected:
	void optFS(Lexer&);
	void closeMiniFS(Lexer&);			 //�ر�ϵͳ
	void formatMiniFS(Lexer&);			 //��ʽ��ϵͳ
	void showHelp(Lexer&);				 //��ʾ����
	void createMiniFS(Lexer&);			 //����MiniFS�ռ�
	void mountMiniFS(Lexer&);			 //װ��MiniFS�ռ�
	void showFSInfo(Lexer&);			 //��ʾϵͳ����
	bool ready();

	bool doOpt(std::vector<int>emptys,MiniFile*);
};