#pragma once
#include "CommandHandler.h"
class HandleDel :
	public CommandHandler
{
public:
	

	void onHandleCommand(Lexer&param)override;
};

