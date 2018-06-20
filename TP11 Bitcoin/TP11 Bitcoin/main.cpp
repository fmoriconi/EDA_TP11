/////////////////////////////////////////////////////////////////
///////////////////AGUANTE BITCOIN VIEJA/////////////////////////
/////////////////////////////////////////////////////////////////

#include <iostream>
#include "AllegroClass.h"
#include "Nodo.h"
#include "parseCmdLine.h"
#include "parameters.h"
#include "Grafo.h"

int main(int argc, char * argv[]) {

	parameters_t params;
	srand(time(NULL));

	if (parseCmdLine(argc, argv, &parseCallback, &params) > 0) {

		Grafo graph(params.nodes, params.miners);
		allegroClass al_class(graph.nodes.size());
		EDAeventHandler ev_handler;

		al_class.drawConnection(graph.nodes);

		EDAevent ev;
		ev.type = EDAEVENT_TYPE::NOEVENT;

		do {
			ev = al_class.getInput(graph.nodes);

			switch (ev.type) {
				case EDAEVENT_TYPE::PAY: {
					for (Nodo* node : graph.nodes) {
						if (ev.id == node->getID()) {
							node->sendTransaction(node->createTransaction(1, 3));
						}
					}
					break;
				}
				case EDAEVENT_TYPE::M_PAY: {

					break;
				}
			}

			al_class.updateDisplay(graph.nodes);
		} while (ev.type != EDAEVENT_TYPE::QUIT);

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