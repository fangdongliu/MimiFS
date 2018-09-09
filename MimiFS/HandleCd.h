#pragma once
#include "CommandHandler.h"
class HandleCd:public CommandHandler
{
public:

	void onHandleCommand(Lexer&param)override;
};

