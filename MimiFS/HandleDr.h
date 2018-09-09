#pragma once
#include "CommandHandler.h"
class HandleDr :
	public CommandHandler
{
public:
	void onHandleCommand(Lexer&param)override;
};

