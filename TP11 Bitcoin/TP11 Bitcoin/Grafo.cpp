#include "Grafo.h"
#include <algorithm>
#include <time.h>


Grafo::Grafo(unsigned int nodeQty,unsigned int minerQty )
{
	srand(time(NULL));

	for (; nodeQty != 0; nodeQty--) { //Creo los nodos y los pusheo al vector.
		if (minerQty != 0) {
			Nodo * node = new Nodo(true);
			this->nodes.push_back(node);
			minerQty--; //Si son mineros, entrara aqui hasta que ya no necesite crear mas mineros.
		}
		else {
			Nodo * node = new Nodo(false);
			this->nodes.push_back(node);
		}
	}

	std::random_shuffle(nodes.begin(), nodes.end()); //Shuffleamos porque no somos aburridos y no queremos que todos los mineros esten juntos.

	enumNodes();
	createConnections();
	ensureGrafoConexo();

}


Grafo::~Grafo()
{
}

void Grafo::enumNodes()
{
	int i = 0;

	for (Nodo * node : this->nodes) {
		node->setID(i);
		i++;
	}
}

void Grafo::createConnections() {

	for (Nodo * node : this->nodes) {

		ID connectTo; //ID del nodo al cual me voy a conectar.
		unsigned int attempts = 0; //Cantidad de intentos de conectarse. Para romper el loop si no se puede conectar.

		while ( (node->connectedNodes.size() < 2) && (attempts <= nodes.size()*5) ) { //Multiplico nodes.size por cinco simplemente para tener mas posibilidades de que le toque conectarse a un nodo con conexiones libres, pues recordemos que es al azar.

			connectTo = (rand() % this->nodes.size()); //Elijo un numero de 0 a la cantidad de nodos - 1.
			
			if (connectTo != node->getID()) { //No puedo conectarme a mi mismo.

				if (node->connectedNodes.size() == 0) { //Si no me conecte a nada, no debo evaluar si ya me conecte

					if (nodes.at(connectTo)->connectedNodes.size() < 2) { //Busco el nodo al cual intento conectarme y me fijo si sigue con conexiones disponibles.
						node->connectedNodes.push_back(this->nodes.at(connectTo)); //Me conecto al nodo.
						nodes.at(connectTo)->connectedNodes.push_back(node);
					}
				}
				else if (node->connectedNodes.at(0) != this->nodes.at(connectTo)) { //Si ya me conecte a otro nodo antes tengo que mirar si me estoy tratando de conectar al mismo.
					if (nodes.at(connectTo)->connectedNodes.size() < 2) { //Y me fijo si tiene conexiones libres.
						node->connectedNodes.push_back(this->nodes.at(connectTo));
						nodes.at(connectTo)->connectedNodes.push_back(node);

					}
				}

			}

			attempts++; //Aumento un intento.

		}

	}

}

void Grafo::ensureGrafoConexo()
{
	std::vector<bool> visited;

	for (Nodo * node : this->nodes)
		visited.push_back(false);

	int i = 0;
	bool allVisited = false;

	while (!allVisited) {

		for (int l = 0; l < visited.size(); l++) {
			visited[l] = false; //Pongo todo en false devuelta.
		}

		recursion(i, visited);
		allVisited = true; //Asumo que se visito todo hasta que se demuestre lo contrario.

		for (bool visit : visited) {
			if (visit == false)
				allVisited = false;
		}

		if (!allVisited) {
			int k = 0;
		
			for (bool visit : visited) {

				if (visit == false) {
					int nodeNumber = connectToVisited(visited);
					nodes.at(k)->connectedNodes.push_back(nodes.at(nodeNumber));
					nodes.at(nodeNumber)->connectedNodes.push_back(nodes.at(k));
					break; // No quiero conectar todos los nodos inconexos, con solo uno alcanza. Vuelvo a probar a ver si persiste el problema.
				}

				k++;

			}
		}
	}

}

bool Grafo::recursion(int index, std::vector<bool>& visited) {


	if (visited[index] == true)
		return true;
	else {

		visited[index] = true;

		int j = 0;

		while (j < nodes.at(index)->connectedNodes.size()) {
			int index_ = nodes.at(index)->connectedNodes.at(j)->getID();
			recursion(index_, visited);
			j++;
		}

	}


}

int Grafo::connectToVisited(std::vector<bool>& visited) {

	bool isVisited = false;
	int vectorPos = NULL;
	while (!isVisited) {

		vectorPos = rand() % visited.size();

		isVisited = visited.at(vectorPos);
	}

	return vectorPos;
}
