#pragma once
#include"CommandHandler.h"
class HandleWrite:public CommandHandler
{
public:
	HandleWrite();
	~HandleWrite();
	void onHandleCommand(Lexer&param)override;
};

