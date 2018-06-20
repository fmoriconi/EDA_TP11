#pragma once

#include <cryptopp\cryptlib.h>
#include "cryptopp\osrng.h"
#include "cryptopp\eccrypto.h"
#include "cryptopp\oids.h"
#include "cryptopp\hex.h"
#include "cryptopp\sha3.h"

typedef double value_t;

class Output
{
public:
	Output();
	~Output();

	value_t amount;
	CryptoPP::DL_PublicKey_EC<CryptoPP::ECP> publicKey;

};

