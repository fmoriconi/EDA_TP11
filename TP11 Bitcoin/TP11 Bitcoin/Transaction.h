#pragma once

#include "Input.h"
#include "Output.h"

#include <cryptopp\cryptlib.h>
#include "cryptopp\osrng.h"
#include "cryptopp\eccrypto.h"
#include "cryptopp\oids.h"
#include "cryptopp\hex.h"
#include "cryptopp\sha3.h"

class Transaction
{
public:
	Transaction();
	~Transaction();

	std::string hashID; //Hash de los cuatro siguientes

	std::vector<Input> inputVector;
	size_t inputQuantity = 0;
	std::vector<Output> outputVector;
	size_t outputQuantity = 0;

	void pushInput(Input input) { inputVector.push_back(input); inputQuantity++; }
	void pushOutput(Output output) { outputVector.push_back(output); outputQuantity++; }

	std::string hashTransaction() {
		CryptoPP::SHA256 hash;
		std::string message;
		for (Input& input : inputVector) {
			message += input.hashID;
			message += input.index;
			message += byteVectorToString(input.signature);
		}
		message += inputQuantity;
		for (Output& output : outputVector) {
			message += output.amount;
			message += byteVectorToString(output.publicKey);
		}
		message += outputQuantity;
		std::string digest;
		CryptoPP::StringSource s(message, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
		hashID = digest;
		return digest;
	}

	std::string byteVectorToString(std::vector<byte> byteVector) {
		std::string returnString;
		for (byte by : byteVector)
			returnString += by;
		return returnString;
	}

	void clean() {
		this->hashID.clear();
		this->inputVector.clear();
		this->outputVector.clear();
		this->inputQuantity = 0;
		this->outputQuantity = 0;
	}

};

