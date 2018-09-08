#pragma once
#include "MiniFolder.h"
class RootFolder :
	public MiniFolder
{
public:
	RootFolder();
	virtual ~RootFolder();

	void load();
};

