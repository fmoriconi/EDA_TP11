#include "Grafo.h"
#include <algorithm>
#include <time.h>


Grafo::Grafo(unsigned int nodeQty,unsigned int minerQty )
{
	srand(time(NULL));

	for (; nodeQty != 0; nodeQty--) { //Creo los nodos y los pusheo al vector.
		if (minerQty != 0) {
			Nodo node(true);
			this->nodes.push_back(node);
			minerQty--; //Si son mineros, entrara aqui hasta que ya no necesite crear mas mineros.
		}
		else {
			Nodo node(false);
			this->nodes.push_back(node);
		}
	}

	std::random_shuffle(nodes.begin(), nodes.end()); //Shuffleamos porque no somos aburridos y no queremos que todos los mineros esten juntos.

	enumNodes();
	createConnections();
}


Grafo::~Grafo()
{
}

void Grafo::enumNodes()
{
	int i = 0;

	for (Nodo node : this->nodes) {
		node.setID(i);
		i++;
	}
}

void Grafo::createConnections() {

	for (Nodo node : this->nodes) {

		ID connectTo; //ID del nodo al cual me voy a conectar.
		unsigned int attempts = 0; //Cantidad de intentos de conectarse. Para romper el loop si no se puede conectar.

		while ( (node.connectedNodes.size() < 2) && (attempts <= nodes.size()) ) {

			connectTo = (rand() % this->nodes.size()); //Elijo un numero de 0 a la cantidad de nodos - 1.
			
			if (connectTo != node.getID()) { //No puedo conectarme a mi mismo.

				if (node.connectedNodes.size() == 0) { //Si no me conecte a nada, no debo evaluar si ya me conecte

					if (nodes.at(connectTo).connectedNodes.size() < 2) { //Busco el nodo al cual intento conectarme y me fijo si sigue con conexiones disponibles.
						node.connectedNodes.push_back(&(this->nodes.at(connectTo))); //Me conecto al nodo.

					}
				}
				else if (node.connectedNodes.at(0) != &(this->nodes.at(connectTo))) { //Si ya me conecte a otro nodo antes tengo que mirar si me estoy tratando de conectar al mismo.
					if (nodes.at(connectTo).connectedNodes.size() < 2) { //Y me fijo si tiene conexiones libres.
						node.connectedNodes.push_back(&(this->nodes.at(connectTo)));

					}
				}

			}

			attempts++; //Aumento un intento.

		}

	}

}
