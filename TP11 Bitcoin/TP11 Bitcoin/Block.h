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

	std::vector<Transaction> transactions;
	long unsigned transactionQuantity = 0;
	long unsigned blockID;
	std::string previousHashID;
	std::string nonce;
	std::string timestamp;

};

