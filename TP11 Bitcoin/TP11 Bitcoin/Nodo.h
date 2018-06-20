#pragma once

#include <cryptopp\cryptlib.h>
#include "cryptopp\osrng.h"
#include "cryptopp\eccrypto.h"
#include "cryptopp\oids.h"
#include "cryptopp\hex.h"
#include "cryptopp\sha3.h"

#include <vector>
#include "UTXO.h"
#include "Blockchain.h"

#define PRIVATE_KEY_CHARS 20
#define PUBLIC_KEY_CHARS 40
#define TARGET_DIFFICULTY 32

class Nodo
{
public:
	Nodo(bool isMiner_);
	~Nodo();

	/*if A wants to send some BTC to B, then 2 things will occur:

(a) A will form the input script of the new transaction. It will reference the output script of a previous transaction where A received some BTC (in the past). It will prove the ownership of those BTC by using its Public Key and verifying the Signature.

(b) The output script for the new transaction will be formed by A's client. It will store the hash of the BTC Address of B and enable it to spend these BTC in future.*/

///ANATOMIA DEL NODO
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
	/*Key privada, sirve para crear una firma junto a los datos que queres firmar, para decir que esto es tuyo. La firma
	se denomina Signature.*/
	CryptoPP::DL_PublicKey_EC<CryptoPP::ECP> publicKey;
	/*La key publica, en bitcoin cambia cada vez que haces una transaccion por seguridad, pero en este tp no. Se genera
	a partir de la key privada. Con los datos a verificar, la public key y la signature se verifica si los datos corresponden
	a la private key que se cree haberlo firmado.*/
	std::vector<Transaction> transactionQueue; //cola de transacciones para meter en un bloque
	std::vector<UTXO> UTXOs; //Cantidad de outputs sin gastar del nodo
	std::vector<Nodo*> connectedNodes; //Nodos conectados
	Blockchain blockchain; //La blockchain
	Block blockStack; //Espacio para formar bloques nuevos
	Transaction transactionStack; //Espacio para formar transacciones nuevas

	///FUNCIONES DEL NODO
	std::vector<byte> getSignature(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey &privKey, std::string &data);
	/*Con esta funcion logramos que el nodo firme datos con su key privada*/
	bool verifySignature(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey &pubKey, std::string &data, std::vector<byte> &signature);
	/*Esta funcion nos permite a partir de la data que queremos verificar, la firma del que lo firmo, y el public key del que lo firmo, saber
	si la public key y la signature vienen de la misma private key que se cree que lo firmo.*/

	void receiveTransaction(Transaction & TX);
	bool verifyTransaction(Transaction & TX);
	bool receiveBlock(Block block);
	void sendBlock(Block block);
	void createTransaction(value_t amount, unsigned);

	//mine(); //Minar = Probar un solo valor por nodo minero en cada loop.
	//createBlock();

	std::string hashSome(std::string data);
	std::vector<byte> getPrivateKey();
	std::vector<byte> getPublicKey();
	CryptoPP::DL_PublicKey_EC<CryptoPP::ECP> getPublicKeyRaw() { return this->publicKey; }
	std::string byteVectorToString(std::vector<byte> byteVector);
	std::string stringToHex(std::string string);
	std::vector<UTXO> findUTXOs(value_t val, valueTypes valueType);
	/*Prepara los UTXOs para realizar un output y los manda a la output queue*/

	bool selected;
	unsigned amountOfNodes = 0;
	void setID(ID id_) { (this->id = id_); }
	ID getID() { return this->id; }
	value_t getBalance() { return this->EDAcoinsBalance; }
	bool getIsMiner() { return this->isMiner; }
	void setAmountOfNodes(unsigned int amount) { this->amountOfNodes = amount; }

private:

	bool isMiner = false; //Esto se decide al momento de construir y no vuelve a modificarse.

	value_t EDAcoinsBalance; //El total de EDACoins que tiene el nodo.
	void updateBalance();

	int findClosestBiggerValue(value_t val, valueTypes valueType);
	std::vector<UTXO> sumLowerValues(value_t val, valueTypes valueType);
	ID id;
	Nodo * searchForNode(ID id, std::vector<bool>& visited);
};