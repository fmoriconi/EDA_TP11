#pragma once

#include "Block.h"

class Blockchain
{
public:
	Blockchain();
	~Blockchain();

	std::vector<Block> blockchain;
};

