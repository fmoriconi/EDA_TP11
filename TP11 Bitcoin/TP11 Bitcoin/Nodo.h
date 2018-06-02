#pragma once

#include <vector>
#include "UTXO.h"

class Nodo
{
public:
	Nodo(bool isMiner_);
	~Nodo();


	//receiveTX(TX, PubKey);
	//receiveBlock(Block, PubKey);

	//checkedTX(bool, Tx);
	//checkedBlock( , );


	////Si el nodo es minero

	//mine(); //Minar = Probar un solo valor por nodo minero en cada loop.
	//createBlock();


	std::vector<UTXO> inputQueue;
	std::vector<UTXO> outputQueue;
	std::vector<UTXO> UTXOs;

	std::vector<Nodo*> connectedNodes;

	bool prepareOutputTransaction(value_t val, valueTypes valueType);

	bool selected;
	void setID(ID id_) { (this->id = id_); }
	ID getID() { return this->id; }
	value_t getBalance() { return this->EDAcoinsBalance; }
	bool getIsMiner() { return this->isMiner; }

private:

	bool isMiner = false; //Esto se decide al momento de construir y no vuelve a modificarse.
	
	value_t EDAcoinsBalance; //El total de EDACoins que tiene el nodo.
	void updateBalance();

	int findClosestBiggerValue(value_t val, valueTypes valueType);
	bool sumLowerValues(value_t val, valueTypes valueType);
	ID id;


};

