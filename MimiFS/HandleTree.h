#pragma once
#include "CommandHandler.h"
class HandleTree :
	public CommandHandler
{
public:

	void onHandleCommand(Lexer&param)override;
};

