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

	std::vector<Transaction> getTransactions() { return transactions; }
	long unsigned getBlockID() { return blockID; }
	std::string getPreviousHashID() { return previousHashID; }
	uint32_t getNonce() { return nonce; }
	std::string getTimestamp() { return timestamp; }

	void setNonce(uint32_t nonce_) { nonce = nonce_; }
	void setTimestamp(std::string timestamp_) { timestamp = timestamp_; }
	void setPreviousHashID(std::string previousHashID_) { previousHashID = previousHashID_; }
	void setBlockID(long unsigned blockID_) { blockID = blockID_; }

	void clean() {
		transactions.clear();
		transactionQuantity = 0;
		blockID = 0;
		previousHashID.clear();
		nonce = 0;
		timestamp.clear();
	}

	void pushTransaction(Transaction TX) {
		transactions.push_back(TX);
		transactionQuantity++;
	}

	std::string hashBlock() {
		CryptoPP::SHA256 hash;
		std::string message;
		message += this->timestamp;
		message += this->nonce;
		message += this->previousHashID;
		message += std::to_string(this->blockID);
		message += std::to_string(this->transactionQuantity);
		for (Transaction& TX : transactions) {
			message += TX.hashTransaction();
		}
		std::string digest;
		CryptoPP::StringSource s(message, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
		return digest;
	}
	
	std::vector<Transaction> transactions;
	long unsigned transactionQuantity = 0;
	long unsigned blockID;
	std::string previousHashID;
	uint32_t nonce;
	std::string timestamp;

};

