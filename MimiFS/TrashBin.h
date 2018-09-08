#pragma once
#include "MiniFolder.h"
class TrashBin :
	public MiniFolder
{
public:
	TrashBin();
	virtual ~TrashBin();

	void load();
};

