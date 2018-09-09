#pragma once
#include "CommandHandler.h"
class HandleMv:public CommandHandler
{
public:
	HandleMv();
	~HandleMv();
	void onHandleCommand(Lexer&param)override;
};

