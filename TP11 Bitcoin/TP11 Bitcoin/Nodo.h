#pragma once
#include "cryptlib.h"
#include "eccrypto.h"
#include "ecp.h"
#include "sha.h"

#include <vector>
#include "UTXO.h"

class Nodo
{
public:
	Nodo(bool isMiner_);
	~Nodo();

	/*En estos comentarios voy a poner lo que aprendi buscando como hacer esto*/
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
	CryptoPP::DL_PublicKey_EC<CryptoPP::ECP> publicKey;

	/*Bueno, el tp dice que un nodo tiene una pubkey y una privkey, aca las hice. Las cosas de cryptopp estan todas
	metidas en el namespace de cryptopp. Al parecer el ECDSA, es el Elliptic Curve Digital Signature Algorithm, que segun
	lo que entendi es el algoritmo que nos hace las keys. Es un template y tiene como primer parametro un "EC" que es una
	Elliptic Curve, y como segundo parametro un "H" que es un algoritmo de hasheo, en nuestro caso, SHA256.
	
	La privateKey es como un objeto y la public key de tipo "DL_PublicKey_EC" que tiene como parametro un "EC" se genera
	a partir de la publicKey con la funcion MakePublicKey().*/

	/*Por ahora no linkea bien*/



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



//receiveTX(TX, PubKey);
//receiveBlock(Block, PubKey);

//checkedTX(bool, Tx);
//checkedBlock( , );


////Si el nodo es minero

//mine(); //Minar = Probar un solo valor por nodo minero en cada loop.
//createBlock();