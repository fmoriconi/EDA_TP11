#pragma once

#include <queue>
#include "Grafo.h"

enum class EDAEVENT_TYPE {QUIT, PAY, M_PAY, MINED, NOEVENT };

typedef struct EDAevent {
	EDAEVENT_TYPE type = EDAEVENT_TYPE::NOEVENT;
		unsigned id = 0;
};

class EDAeventHandler {
public:
	void pushEvent(EDAevent ev_) { events.push(ev_); }
	EDAevent getEvent(EDAevent ev_) { EDAevent ev = events.back(); events.pop(); return ev; }
	bool isEmpty() { return events.empty(); }
	void itsTimeToOilUp(Grafo& graph); //El verdadero handler. Es quien hace la magia y procesa todo.

private:
	std::queue<EDAevent> events;
};
