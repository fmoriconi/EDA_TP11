#pragma once

#include <vector>
#include "Output.h"
#include "Input.h"
#include "UTXO.h"
#include "Transaction.h"

class Block
{
public:
	Block();
	~Block();

private:
	std::vector<Transaction> transactions;
	std::string previousHashID;
	std::string nonce;
	std::string timestamp;

};

