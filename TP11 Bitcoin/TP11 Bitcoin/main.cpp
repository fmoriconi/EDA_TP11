
/////////////////////////////////////////////////////////////////
///////////////////AGUANTE BITCOIN VIEJA/////////////////////////
/////////////////////////////////////////////////////////////////

#include <iostream>
#include "AllegroClass.h"
#include "Nodo.h"
//#include "cryptopp"
int main(int argc, char * argv[]) {

	allegroClass al_class(4);
	std::vector<Nodo*> nodos;

	Nodo n1(true), n2(false), n3(false), n4(false);

	n1.connectedNodes.push_back(&n2);
	n1.connectedNodes.push_back(&n3);
	n1.connectedNodes.push_back(&n4);
	n1.selected = true;

	n2.connectedNodes.push_back(&n1);
	n2.connectedNodes.push_back(nullptr);
	n2.connectedNodes.push_back(nullptr);

	n3.connectedNodes.push_back(&n1);
	n3.connectedNodes.push_back(&n4);
	n3.connectedNodes.push_back(nullptr);

	n4.connectedNodes.push_back(&n3);
	n4.connectedNodes.push_back(nullptr);
	n4.connectedNodes.push_back(nullptr);

	nodos.push_back(&n1);
	nodos.push_back(&n2);
	nodos.push_back(&n3);
	nodos.push_back(&n4);

	al_class.updateDisplay(nodos);

	getchar();

	//createNodes();

	//while (checkConnectivity()) //BFS o DFS...
	//	makeConnections();

	//for (Node N in Graph) {

	//	if (N.hasTX2Send())
	//		N.sendTX();

	//} //Primero se envian todas las transacciones, apilandose en una cola como en networking, y luego se procesan.

	//for (Node N in Graph) {
	//	if (N.TXArrived)
	//		N.VerifyTxQueue();
	//}

	//for (N in graph) {
	//	if (N.isMiner())
	//		N.OneMine(); //ONE MIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIINE
	//}


	//createTX(N1, N2, 7); //Mando 7 EDACoins del nodo 1 al nodo 2

	//----------------------------//
	// ALLEGRO	- Input y eventos //
	//----------------------------//

}

//Si nos da una embolia, revisar bitcoinbook en gitHub. Sirve para entender los conceptos.

//Para verificar la transaccion tiene que estar en el UTXO y si no esta en la UTXO, ver si esta en PENDING.
//Si esta en la UTXO, buscamos la transaccion en el blockchain y validamos toda la transaccion.
//Si esta en PENDING, una transaccion de pending referencia a algo en el blockchain de UTXO. Esto se cumples siempre.
//Verificar coherencia de la entrada y salida de transaccion. Si entran dos, salen dos.
//La clave publica esta en la output transaction.
// {prevUTXO, Sign}, {pubKey, $} (?)
//