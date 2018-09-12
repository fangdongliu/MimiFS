#pragma once
#include "CommandHandler.h"
class HandleCp :
	public CommandHandler
{
public:
	HandleCp();
	~HandleCp();
	void onHandleCommand(Lexer&param)override;
	void doCopy(MiniFile*,MiniFolder*);

	int cnt1;
	int cnt2;
};

