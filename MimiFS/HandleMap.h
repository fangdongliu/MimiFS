#pragma once
#include "CommandHandler.h"
class HandleMap :
	public CommandHandler
{
public:
	HandleMap();
	~HandleMap();
	void onHandleCommand(Lexer&param)override;
};

