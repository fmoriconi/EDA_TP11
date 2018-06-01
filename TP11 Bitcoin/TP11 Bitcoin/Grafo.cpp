#include "Grafo.h"
#include <algorithm>



Grafo::Grafo(unsigned int nodeQty,unsigned int minerQty )
{

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
