#include "stdafx.h"
#include "RootFolder.h"


RootFolder::RootFolder()
{
	fileHead.blockId = 1;
	parent = 0;
}


RootFolder::~RootFolder()
{
}

void RootFolder::load() {

	MiniFolder::load();

}