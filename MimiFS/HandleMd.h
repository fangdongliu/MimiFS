#pragma once
#include "CommandHandler.h"
class HandleMd :
	public CommandHandler
{
public:
	void onHandleCommand(std::istream&param)override;
};

