#pragma once
#include "CommandHandler.h"

class HandleExport :
	public CommandHandler
{
public:
	HandleExport();
	~HandleExport();
	void onHandleCommand(Lexer&param)override;
	void doExport(std::string& relativePath, MiniFile*, std::experimental::filesystem::path&winPath);
private:
	int cnt1;
	int cnt2;
};

