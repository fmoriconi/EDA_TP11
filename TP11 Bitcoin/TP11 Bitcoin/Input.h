#pragma once
#include <vector>

#include <cryptopp\cryptlib.h>
#include "cryptopp\osrng.h"
#include "cryptopp\eccrypto.h"
#include "cryptopp\oids.h"
#include "cryptopp\hex.h"
#include "cryptopp\sha3.h"

class Input
{
public:
	Input(std::string hashID_, long unsigned index_, std::vector<byte> signature_);
	~Input();

	std::string hashID;
	long unsigned index;
	std::vector<byte> signature;

	bool godSent;

};

