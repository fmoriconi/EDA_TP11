#pragma once

#include <vector>
#include "Nodo.h"

class Grafo
{
public:
	Grafo(unsigned int nodeQty, unsigned int minerQty);
	~Grafo();

	std::vector<Nodo> nodes; // Vector con los nodos

private:
	void enumNodes();
	void createConnections();
};

