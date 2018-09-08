#include "stdafx.h"
#include "TrashBin.h"


TrashBin::TrashBin()
{
	fileHead.blockId = 2;
	parent = 0;
}


TrashBin::~TrashBin()
{
}

void TrashBin::load() {
	MiniFolder::load();
}