#include "UTXO.h"


UTXO::UTXO(value_t amount_, std::string hashID_, long unsigned index_) : hashID(hashID_), index(index_)//Asigna el valor a 
{
	if (amount_ > 0)
		this->amount = amount_;
	else
		this->amount = 0; //Value_t enrealidad es un doble, a los cuales no se les puede forzar el signo, por lo que si llega un signo negativo ledamos valor 0.
}


UTXO::~UTXO()
{
}

value_t UTXO::getAmount(valueTypes type) //Devuelve el valor en Ressels.
{
	if (type == valueTypes::EDACoins)
		return this->amount;
	else
		return (this->amount/FRACTIONVALUE);
}
