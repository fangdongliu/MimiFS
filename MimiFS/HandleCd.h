#pragma once
#include "CommandHandler.h"
class HandleCd:public CommandHandler
{
public:

	void onHandleCommand(std::istream&param)override;
};

