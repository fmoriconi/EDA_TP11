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

bool Nodo::prepareOutputTransaction(value_t val, valueTypes valueType)
{
		if (val > ((this->EDAcoinsBalance)*((int) valueType))){ //Este casteo es para ajustar el valor que tendo de EDACoins a Ressels.
			return false;
		}
		else {

			int i = findClosestBiggerValue(val, valueType);

			if (i < this->UTXOs.size()) { //Si i es igual, findClosestBiggerValue no encontro valor mayor.
				this->outputQueue.push_back((this->UTXOs).at(i)); //Pongo el UTXO en el vector de salida.
				this->UTXOs.erase(UTXOs.begin() + i); //Elimino el elemento del vector pues ahora esta enfilado para enviarse.
				return true;
			}

			sumLowerValues(val, valueType); //Si no retornee antes, es porque no hay valor mayor. Esta funcion ya acomoda los vectores.
			return true;

		}

}

void Nodo::updateBalance()
{
	this->EDAcoinsBalance = 0;
	for (UTXO utxo : this->UTXOs)
		EDAcoinsBalance += utxo.getUTXOval(valueTypes::EDACoins); //Sumo el valor de cada UTXO.

}

int Nodo::findClosestBiggerValue(value_t val, valueTypes valueType)
{
	value_t closestBiggerValue = 0; //Valor de la UTXO mayor pero mas cercana al valor
	bool firstBiggerValue = true; //Booleano para setear el primer valor mayor.
	int returnIndex = this->UTXOs.size(); //El indice no puede ser al size pues el indice arranca en 0.

	for (UTXO utxo : this->UTXOs) { //Este FOR encuentra el UTXO con el valor mas grande mas cercano

		int i = 0;

		if ( ( utxo.getUTXOval(valueType) * ((int)valueType) ) >= val) { //Tambien puede ser igual.

			if (firstBiggerValue) { //Seteo el primer mayor valor.
				firstBiggerValue = false;
				closestBiggerValue = utxo.getUTXOval(valueType);
				returnIndex = i;
			}

			if (utxo.getUTXOval(valueType) < closestBiggerValue) {
				closestBiggerValue = utxo.getUTXOval(valueType);
				returnIndex = i;
			}

		}
	}
	return returnIndex; //Si no hubo valor mas grande, devuelve un valor imposible de acceder.
}

bool Nodo::sumLowerValues(value_t val, valueTypes valueType)
{
	value_t totalValue = 0;
	int i = 0;

	for (UTXO utxo : this->UTXOs) { //Este FOR encuentra el UTXO con el valor mas grande mas cercano

			if ( (utxo.getUTXOval(valueType)*(int) valueType) < val) {

				this->outputQueue.push_back((this->UTXOs).at(i)); //Pongo el UTXO en el vector de salida.
				this->UTXOs.erase(UTXOs.begin() + i); //Elimino el elemento del vector pues ahora esta enfilado para enviarse.
				totalValue += utxo.getUTXOval(valueType);

			}

			if (totalValue > val) //Si supere el total, ya no debo sumar mas.
				return true; //Salgo del for. Solo devuelvo BOOL para poder poner un return.

			i++;

	}
	return true;
}
