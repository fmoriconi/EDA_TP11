#include "EDAevent.h"

void EDAeventHandler::itsTimeToOilUp(Grafo& graph)
{
	EDAevent ev = events.front();
	events.pop();


	//Aca es donde metemos si alguien pago, minear, etc. Siempre desencolando de la cola de eventos.

	switch (ev.type) {
	case EDAEVENT_TYPE::PAY: {
		for (Nodo * node : graph.nodes) {
			if (ev.id == node->getID()) {
				node->sendTransaction(node->createTransaction(1, 3));
			}
		}
		break;
	}
	case EDAEVENT_TYPE::M_PAY: {
		///Caso de pago malicioso
		break;
	}
	case EDAEVENT_TYPE::MINED: {

		break;
	}

	}
}
