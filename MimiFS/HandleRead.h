#pragma once
#include "CommandHandler.h"
class HandleRead :
	public CommandHandler
{
public:
	HandleRead();
	~HandleRead();
	void onHandleCommand(Lexer&param)override;
};

