#pragma once
#include "CommandHandler.h"
class HandleRename :
	public CommandHandler
{
public:
	HandleRename();
	~HandleRename();

	void onHandleCommand(Lexer&param)override;

};

