#pragma once

#define FRACTIONVALUE 100
typedef double value_t;
typedef unsigned int ID;

enum class valueTypes {EDACoins = 1, Ressels = FRACTIONVALUE};

class UTXO
{
public:
	UTXO(value_t value_, ID idnumber_); //El numero de ID correspondera a su posicion en el vector UTXO del nodo.
	~UTXO();

	value_t getUTXOval(valueTypes type); //Devuelve la moneda 

private:
	value_t value; //Valor en EDACoins, es privada pues no puede modificarse tras ser creada.
	ID idNumber;
};

