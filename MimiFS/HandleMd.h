#pragma once
#include "CommandHandler.h"
class HandleMd :
	public CommandHandler
{
public:
	void onHandleCommand(Lexer&param)override;
};

