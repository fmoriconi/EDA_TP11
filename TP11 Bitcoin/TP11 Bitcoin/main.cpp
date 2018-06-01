
/////////////////////////////////////////////////////////////////
///////////////////AGUANTE BITCOIN VIEJA/////////////////////////
/////////////////////////////////////////////////////////////////

#include <iostream>
#include "parseCmdLine.h"
#include "parameters.h"

//#include "cryptopp"

int main(int argc, char * argv[]) {

	parameters_t params;

	if (parseCmdLine(argc, argv, &parseCallback, &params) > 0) {

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

		////----------------------------//
		//// ALLEGRO	- Input y eventos //
		////----------------------------//

		getchar();

	}

	return 0;
}

//Si nos da una embolia, revisar bitcoinbook en gitHub. Sirve para entender los conceptos.

//Para verificar la transaccion tiene que estar en el UTXO y si no esta en la UTXO, ver si esta en PENDING.
//Si esta en la UTXO, buscamos la transaccion en el blockchain y validamos toda la transaccion.
//Si esta en PENDING, una transaccion de pending referencia a algo en el blockchain de UTXO. Esto se cumples siempre.
//Verificar coherencia de la entrada y salida de transaccion. Si entran dos, salen dos.
//La clave publica esta en la output transaction.
// {prevUTXO, Sign}, {pubKey, $} (?)
//