#pragma once
#include "CommandHandler.h"
class HandleAtt :
	public CommandHandler
{
public:
	HandleAtt();
	~HandleAtt();
	void onHandleCommand(Lexer&param)override;
};

