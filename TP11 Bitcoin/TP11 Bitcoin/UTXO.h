#pragma once

#include <string>

#define FRACTIONVALUE 100
typedef double value_t;
typedef unsigned int ID;

enum class valueTypes {EDACoins = 1, Ressels = FRACTIONVALUE};

class UTXO
{
public:
	UTXO(value_t amount_, std::string hashID_, long unsigned index_);
	~UTXO();

	value_t getAmount(valueTypes type); //Devuelve la moneda 
	std::string getHashID() { return hashID; }
	long unsigned getIndex() { return index; }

private:
	value_t amount; //Valor en EDACoins, es privada pues no puede modificarse tras ser creada.
	std::string hashID;
	long unsigned index;
};

