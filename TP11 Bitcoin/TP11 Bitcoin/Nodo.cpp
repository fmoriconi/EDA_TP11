#include "Nodo.h"



Nodo::Nodo(bool isMiner_)
{
	if (isMiner_)
		this->isMiner = true;
	else
		this->isMiner = false;

	CryptoPP::AutoSeededRandomPool ASRP;
	privateKey.Initialize(ASRP, CryptoPP::ASN1::secp160r1());
	bool result = privateKey.Validate(ASRP, 3);
	if (!result)
		std::cout << "private key from node " << this->id << " is not valid!" << std::endl;
	else
		privateKey.MakePublicKey(this->publicKey);
}


Nodo::~Nodo()
{

}

std::vector<byte> Nodo::getSignature(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey &privKey, std::string &data)
{
	CryptoPP::AutoSeededRandomPool ASRP;
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer Signer(privKey);
	bool result = Signer.AccessKey().Validate(ASRP, 3);
	if (!result)
		std::cout << "failed to create signer node id " << this->id << std::endl;
	size_t signatureLength = Signer.MaxSignatureLength();
	std::vector<byte> signature(signatureLength, 0x00);
	signatureLength = Signer.SignMessage(ASRP, (const byte*)data.data(), data.size(), (byte*)signature.data());
	return signature;
}

bool Nodo::verifySignature(CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey & pubKey, std::string & data, std::vector<byte>& signature)
{
	CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier(pubKey);
	return  verifier.VerifyMessage((const byte*)data.data(), data.size(), (const byte*)signature.data(), signature.size());
}

void Nodo::receiveTransaction(Transaction& TX)
{ 
	if (verifyTransaction(TX)) {
		for (Nodo* nodo : connectedNodes) {
			if (nodo != nullptr) {
				nodo->receiveTransaction(TX); //this doesnt sound right
			}
		}
	}
}

bool Nodo::verifyTransaction(Transaction & TX)
{
	//Que inputs y outputs no esten vacios
	if (TX.inputVector.empty())
		return false;
	if (TX.outputVector.empty())
		return false;
	//Que ningun input tenga hash 0
	for (Input& input : TX.inputVector) {
		if (std::stoi(input.hashID) == 0)
			return false;
	}
	//Que la transaccion no este presente ya en algun bloque de la blockchain
	for (Block& block : blockchain.blockchain) {
		for (Transaction& TX2 : block.transactions) {
			if (TX.hashID == TX2.hashID)
				return false;
		}
	}
	//Que la cantidad de EDAcoins en inputs sea la misma que en outputs
	unsigned outputTotal = 0;
	unsigned inputTotal = 0;
	for (Output& output : TX.outputVector) { //Sumo amounts de outputs
		outputTotal += output.amount;
	}
	for (Input& input : TX.inputVector) { //Busco para cada input de la TX a verificar
		for (Block& block : blockchain.blockchain) { //Si dentro de cada bloque de la blockchain
			for (Transaction& TX : block.transactions) { //Dentro de cada TX de cada bloque
				if (TX.hashID == input.hashID) { //Hay alguna transaccion con el mismo hash que el que referencia el input
					std::string data = input.hashID + std::to_string(input.index);
					if (verifySignature(TX.outputVector[input.index].publicKey, data, input.signature)) {
						inputTotal += TX.outputVector[input.index].amount; //Y si la hay, es porque se encontro el output al que referencia la input y sumo amount
					}
					else
						return false;
				}
			}
		}
	}
	if (outputTotal != inputTotal)
		return false;

	unsigned index = 0;
	for (Output& output : TX.outputVector) {
		if (output.publicKey == this->publicKey) { //Es mia
			UTXO myNewUTXO(output.amount, TX.hashTransaction(), index);
			this->UTXOs.push_back(myNewUTXO);
		}
		index++;
	}

	return true;
}

bool Nodo::receiveBlock(Block block)
{
	bool iDontHave = true;
	std::string data;

	for (int i = 0; i < blockchain.blockchain.size(); i++) {
		if (blockchain.blockchain[i].blockID == block.blockID)
			iDontHave = false;
	}

	if (iDontHave) {

		for (int i = 0; i < TARGET_DIFFICULTY; i++) { //Verifico si el previous hash es correcto
			if (block.previousHashID[i] != 0)
				return false;
		}

		for (Transaction& TX : block.transactions) { //Hago el hash del bloque
			data += TX.hashTransaction();
		}
		data += block.blockID;
		data += block.nonce;
		data += block.previousHashID;
		data += block.timestamp;
		data += block.transactionQuantity;
		std::string blockHash = hashSome(data);
		for (int i = 0; i < TARGET_DIFFICULTY; i++) { //Verifico si el hash es correcto
			if (blockHash[i] != 0)
				return false;
		}
		for (Transaction& TX : block.transactions) {
			if (!(verifyTransaction(TX)))
				return false;
		}
		//Borro cualquier transaccion se halle en el pool que ya esten en el bloque confirmado
		unsigned index = 0;
		for (Transaction& TX : transactionQueue) {
			for (Transaction& TX2 : block.transactions) {
				if (TX.hashID == TX2.hashID) {
//					std::vector<Transaction>::iterator currTX = transactionQueue.begin() + index;
//					transactionQueue.erase(currTX);
				}
			}
			index++;
		}

		blockchain.blockchain.push_back(block);

		sendBlock(block);

		return true;
	}

	else
		return false;
}

bool Nodo::mine() {
	std::chrono::system_clock::time_point timepoint = std::chrono::system_clock::now();
	std::string timestamp = this->time_point_to_string(timepoint);
	uint32_t nonce = rand();
	Block block = createBlock(nonce, timestamp);
	std::string blockHash = block.hashBlock();
	for (int i = 0; i < 4; i++) {
		if (blockHash[i] != 0)
			return false;
	}
	sendBlock(block);
	return true;
}

Block Nodo::createBlock(uint32_t nonce, std::string timestamp)
{
	blockStack.clean();
	
	for (Transaction TX : transactionQueue) {
		blockStack.pushTransaction(TX);
	}
	blockStack.setNonce(nonce);
	blockStack.setTimestamp(timestamp);
	blockStack.setPreviousHashID(blockchain.blockchain.back().hashBlock());
	blockStack.setBlockID(blockchain.blockchain.back().getBlockID() + 1);
	return blockStack;
}

void Nodo::sendBlock(Block block)
{
	for (Nodo* nodo : connectedNodes) {
		if (nodo != nullptr) {
			nodo->receiveBlock(block); //this doesnt sound right
		}
	}
}

Transaction Nodo::createTransaction(value_t amount, unsigned nodeID)
{
	std::vector<bool> visited;
	for (int i = 0; i < amountOfNodes; i++);
		visited.push_back(false);

	Nodo * nodePtr = searchForNode(nodeID, visited); //Encuentro a quien se lo quiero mandar

	this->transactionStack.clean(); //Limpio lugar donde creo transaction nueva
	if (nodePtr) {
		CryptoPP::DL_PublicKey_EC<CryptoPP::ECP> pubKeyRecipient = nodePtr->getPublicKeyRaw(); //Encuentro la pubKey del recipient

		std::vector<UTXO> chosenUTXOs = findUTXOs(amount, valueTypes::EDACoins); //Encuentro los UTXOs necesarios para completar amount del sender.

		unsigned realAmount = 0;

		for (UTXO& utxo : chosenUTXOs) {
			std::string data = (utxo.getHashID() + std::to_string(utxo.getIndex())); //Meto a la transaction las inputs
			Input input(utxo.getHashID(), utxo.getIndex(), this->getSignature(this->privateKey, data));
			transactionStack.pushInput(input);

			realAmount += utxo.getAmount(valueTypes::EDACoins); //Calculo la plata que verdaderamente estoy poniendo sobre la mesa
		}

		if (realAmount > amount) {
			Output realOutput(amount, pubKeyRecipient); //Mando la plata que tengo que mandar
			Output changeOutput(realAmount - amount, this->publicKey); //Me devuelvo el exceso
			transactionStack.pushOutput(realOutput);
			transactionStack.pushOutput(changeOutput);
		}
		else {
			Output realOutput(amount, pubKeyRecipient); //Mando la plata que tengo que mandar
			transactionStack.pushOutput(realOutput);
		}
	}
	return transactionStack;
}

void Nodo::sendTransaction(Transaction TX)
{
	for (Nodo* nodo : connectedNodes) {
		if (nodo != nullptr) {
			nodo->receiveTransaction(TX);
		}
	}
}

Nodo * Nodo::searchForNode(ID NodeID, std::vector<bool>& visited) {

	if (this->id == NodeID) {
		return this;
	}
	else {
		visited[this->getID()] = true;

		for (Nodo * nodito : connectedNodes) {
			if (!visited[nodito->getID()])
				return nodito->searchForNode(NodeID, visited);
		}
	}

	return nullptr; //Si el ID no existe, devuelve un puntero a NULL.
}

std::string Nodo::hashSome(std::string data)
{
	CryptoPP::SHA256 hash;
	std::string digest;
	CryptoPP::StringSource s(data, true, new CryptoPP::HashFilter(hash, new CryptoPP::HexEncoder(new CryptoPP::StringSink(digest))));
	return digest;
}

std::vector<byte> Nodo::getPrivateKey()
{
	std::vector<byte> privKeyByteArray(PRIVATE_KEY_CHARS);
	const CryptoPP::Integer &privateKeyInteger = this->privateKey.GetPrivateExponent(); //La key posta
	privateKeyInteger.Encode(privKeyByteArray.data(), privateKeyInteger.ByteCount());
	return privKeyByteArray;
}

std::vector<byte> Nodo::getPublicKey()
{
	std::vector<byte> pubKeyByteArray(PUBLIC_KEY_CHARS);
	std::vector<byte> aux(PUBLIC_KEY_CHARS / 2);
	const CryptoPP::ECP::Point &point = this->publicKey.GetPublicElement();
	point.x.Encode(aux.data(), point.x.ByteCount());
	pubKeyByteArray.clear();
	pubKeyByteArray.insert(pubKeyByteArray.end(), aux.begin(), aux.end());
	point.y.Encode(aux.data(), point.y.ByteCount());
	pubKeyByteArray.insert(pubKeyByteArray.end(), aux.begin(), aux.end());
	return pubKeyByteArray;
}

std::string Nodo::byteVectorToString(std::vector<byte> byteVector) {
	std::string returnString;
	for (byte by : byteVector)
		returnString += by;
	return returnString;
}

std::vector<UTXO> Nodo::findUTXOs(value_t val, valueTypes valueType)
{
		std::vector<UTXO> chosenUTXOs;

		if (val > ((this->EDAcoinsBalance)*((int) valueType))){ //Este casteo es para ajustar el valor que tendo de EDACoins a Ressels
		}

		else {

			int i = findClosestBiggerValue(val, valueType);

			if (i < this->UTXOs.size()) { //Si i es igual, findClosestBiggerValue no encontro valor mayor.
				chosenUTXOs.push_back(this->UTXOs.at(i)); //Pongo el UTXO en el vector de salida.
				this->UTXOs.erase(UTXOs.begin() + i); //Elimino el elemento del vector pues ahora esta enfilado para enviarse.
			}

			chosenUTXOs = sumLowerValues(val, valueType); //Si no retornee antes, es porque no hay valor mayor. Esta funcion ya acomoda los vectores.
		}

		return chosenUTXOs;
}

void Nodo::updateBalance()
{
	this->EDAcoinsBalance = 0;
	for (UTXO utxo : this->UTXOs)
		EDAcoinsBalance += utxo.getAmount(valueTypes::EDACoins); //Sumo el valor de cada UTXO.
}

int Nodo::findClosestBiggerValue(value_t val, valueTypes valueType)
{
	value_t closestBiggerValue = 0; //Valor de la UTXO mayor pero mas cercana al valor
	bool firstBiggerValue = true; //Booleano para setear el primer valor mayor.
	int returnIndex = this->UTXOs.size(); //El indice no puede ser al size pues el indice arranca en 0.

	for (UTXO utxo : this->UTXOs) { //Este FOR encuentra el UTXO con el valor mas grande mas cercano

		int i = 0;

		if ( ( utxo.getAmount(valueType) * ((int)valueType) ) >= val) { //Tambien puede ser igual.

			if (firstBiggerValue) { //Seteo el primer mayor valor.
				firstBiggerValue = false;
				closestBiggerValue = utxo.getAmount(valueType);
				returnIndex = i;
			}

			if (utxo.getAmount(valueType) < closestBiggerValue) {
				closestBiggerValue = utxo.getAmount(valueType);
				returnIndex = i;
			}

		}
	}
	return returnIndex; //Si no hubo valor mas grande, devuelve un valor imposible de acceder.
}

std::vector<UTXO> Nodo::sumLowerValues(value_t val, valueTypes valueType)
{
	value_t totalValue = 0;
	int i = 0;
	std::vector<UTXO> chosenUTXOs;

	for (UTXO utxo : this->UTXOs) { //Este FOR encuentra el UTXO con el valor mas grande mas cercano

			if ( (utxo.getAmount(valueType)*(int) valueType) < val) {

				chosenUTXOs.push_back((this->UTXOs).at(i)); //Pongo el UTXO en el vector de salida.
				this->UTXOs.erase(UTXOs.begin() + i); //Elimino el elemento del vector pues ahora esta enfilado para enviarse.
				totalValue += utxo.getAmount(valueType);

			}

			if (totalValue > val) //Si supere el total, ya no debo sumar mas.
				return chosenUTXOs; //Salgo del for. Solo devuelvo BOOL para poder poner un return.

			i++;

	}
	return chosenUTXOs;
}

std::string Nodo::stringToHex(std::string string)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = string.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = string[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}

std::string Nodo::time_point_to_string(std::chrono::system_clock::time_point &tp)
{
	using namespace std;
	using namespace std::chrono;

	auto ttime_t = system_clock::to_time_t(tp);
	auto tp_sec = system_clock::from_time_t(ttime_t);
	milliseconds ms = duration_cast<milliseconds>(tp - tp_sec);

	std::tm * ttm = localtime(&ttime_t);

	char date_time_format[] = "%Y.%m.%d-%H.%M.%S";

	char time_str[] = "yyyy.mm.dd.HH-MM.SS.fff";

	strftime(time_str, strlen(time_str), date_time_format, ttm);

	string result(time_str);
	result.append(".");
	result.append(to_string(ms.count()));

	return result;
}