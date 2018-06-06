#pragma once

#include <cryptopp\cryptlib.h>
#include "cryptopp\osrng.h"
#include "cryptopp\eccrypto.h"
#include "cryptopp\oids.h"
#include "cryptopp\hex.h"
#include "cryptopp\sha3.h"

#include <vector>
#include "UTXO.h"

#define PRIVATE_KEY_CHARS 20
#define PUBLIC_KEY_CHARS 40

class Nodo
{
public:
	Nodo(bool isMiner_);
	~Nodo();

	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
	/*Key privada, sirve para crear una firma junto a los datos que queres firmar, para decir que esto es tuyo. La firma
	se denomina Signature.*/
	CryptoPP::DL_PublicKey_EC<CryptoPP::ECP> publicKey;
	/*La key publica, en bitcoin cambia cada vez que haces una transaccion por seguridad, pero en este tp no. Se genera
	a partir de la key privada. Con los datos a verificar, la public key y la signature se verifica si los datos corresponden
	a la private key que se cree haberlo firmado.*/

	std::vector<byte> getSignature(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey &privKey, std::string &data);
	/*Con esta funcion logramos que el nodo firme datos con su key privada*/

	/*
	COSAS QUE EL NODO TIENE QUE HACCER CUANDO LE LLEGA UN BLOQUE
	verificar hash correcto
	verificar que cumpla el target
	verificar si el previous hash es correcto
	verificar todas las transacciones 
	*/

	bool verifySignature(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey &pubKey, std::string &data, std::vector<byte> &signature);
	/*Esta funcion nos permite a partir de la data que queremos verificar, la firma del que lo firmo, y el public key del que lo firmo, saber
	si la public key y la signature vienen de la misma private key que se cree que lo firmo.*/


	//receiveTX(TX, PubKey);
	//receiveBlock(Block, PubKey);

	//checkedTX(bool, Tx);
	//checkedBlock( , );


	////Si el nodo es minero

	//mine(); //Minar = Probar un solo valor por nodo minero en cada loop.
	//createBlock();


	std::vector<byte> getPrivateKey();
	std::vector<byte> getPublicKey();
	std::string byteVectorToString(std::vector<byte> byteVector);


	std::vector<UTXO> inputQueue;
	std::vector<UTXO> outputQueue;
	std::vector<UTXO> UTXOs;

	std::vector<Nodo*> connectedNodes;

	bool prepareOutputTransaction(value_t val, valueTypes valueType);
	/*Prepara los UTXOs para realizar un output y los manda a la output queue*/

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