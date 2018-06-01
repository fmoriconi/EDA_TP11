#include "UTXO.h"


UTXO::UTXO(value_t value_, ID idNumber_) //Asigna el valor a 
{
	if (value_ > 0)
		this->value = value_;
	else
		this->value = 0; //Value_t enrealidad es un doble, a los cuales no se les puede forzar el signo, por lo que si llega un signo negativo ledamos valor 0.

	this->idNumber = idNumber_;
}


UTXO::~UTXO()
{
}

value_t UTXO::getUTXOval(valueTypes type) //Devuelve el valor en Ressels.
{
	if (type == valueTypes::EDACoins)
		return this->value;
	else
		return (this->value/FRACTIONVALUE);
}
