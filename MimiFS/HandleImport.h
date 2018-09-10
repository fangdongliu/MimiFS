#pragma once
#include "CommandHandler.h"
class HandleImport :
	public CommandHandler
{
public:
	HandleImport();
	~HandleImport();
	void onHandleCommand(Lexer&param)override;
	void doImport( MiniFolder*,const std::experimental::filesystem::path&winPath);
	void importFile(MiniFolder*,const std::experimental::filesystem::path&winPath);
private:
	int cnt1;
	int cnt2;
};

