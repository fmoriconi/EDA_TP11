#pragma once

#include <cryptopp\cryptlib.h>
#include "cryptopp\osrng.h"
#include "cryptopp\eccrypto.h"
#include "cryptopp\oids.h"
#include "cryptopp\hex.h"
#include "cryptopp\sha3.h"

#include <vector>
#include <ctime>
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

	void receiveTransaction(Transaction TX); //Recive una transaccion, la verifica.
	bool verifyTransaction(Transaction TX); //Verifica una transaccion.
	bool receiveBlock(Block block); //Recive bloque nuevo, lo verifica.
	Block createBlock(uint32_t nonce, std::string timestamp); //Crea un bloque nuevo.
	void sendBlock(Block block); //Propaga un bloque a sus conectados.
	Transaction createTransaction(value_t amount, unsigned); //Crea una transaccion nueva
	void sendTransaction(Transaction TX); //Propaga una transaccion a sus conectados.
	bool mine(); //True si pudo minar, false si no pudo minar.

	std::string hashSome(std::string data); //Hashea data
	std::vector<byte> getPrivateKey(); //Devuelve private key en vector de bytes
	std::vector<byte> getPublicKey(); //Devuelve pub key en vector de bytes
	CryptoPP::DL_PublicKey_EC<CryptoPP::ECP> getPublicKeyRaw() { return this->publicKey; } //Devuelve pub key en formato raro
	std::string byteVectorToString(std::vector<byte> byteVector); //De vector de bytes a string
	std::string stringToHex(std::string string); //De string a hexa
	std::vector<UTXO> findUTXOs(value_t val, valueTypes valueType); //Encuentra UTXO buenos para pagar una dada suma.
	/*Prepara los UTXOs para realizar un output y los manda a la output queue*/

	bool selected; //Si este nodo esta seleccionado para ver su informacion
	unsigned amountOfNodes = 0; //Cantridad de nodos que existen
	void setID(ID id_) { (this->id = id_); } //Para setear su id
	ID getID() { return this->id; } //Para getear id
	value_t getBalance() { updateBalance(); return this->EDAcoinsBalance; } //Te da el balance en EDACOINS
	bool getIsMiner() { return this->isMiner; } //Si es minero
	void setAmountOfNodes(unsigned int amount) { this->amountOfNodes = amount; } //Cuantos nodos existen

private:

	bool isMiner = false; //Esto se decide al momento de construir y no vuelve a modificarse.

	value_t EDAcoinsBalance; //El total de EDACoins que tiene el nodo.
	void updateBalance(); //Updatea el balance de bitcoins.
	bool initialTX = true;
	int findClosestBiggerValue(value_t val, valueTypes valueType);
	std::vector<UTXO> sumLowerValues(value_t val, valueTypes valueType);
	std::string time_point_to_string(std::chrono::system_clock::time_point &tp);
	ID id;
	Nodo * searchForNode(ID id, std::vector<bool>& visited); //Funcion recursiva que dada una id de nodo y un vector de booleanos
	//que corresponde a los nodos ya visitados, busca el nodo pedido. y devuelve puntero a el.
};