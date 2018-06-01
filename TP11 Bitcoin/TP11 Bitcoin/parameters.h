#pragma once

#define OPTION_QTY 2


class parameters_t{
public:

	parameters_t() {
		options[0] = { "nodes" };
		options[1] = { "miners" };
	}

	const char * options[OPTION_QTY];

	bool flags[2] = { false }; // Nodes - Miners.

	int nodes;
	int miners;

};