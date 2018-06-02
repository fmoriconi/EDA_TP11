#pragma once

#include <queue>

enum class EDAEVENT_TYPE {QUIT, NODE_SELECTED, PAY, M_PAY, MINED, AGREGARSEGUNSENECESITE };

typedef struct EDAevent {
	EDAEVENT_TYPE type;
};

class EDAeventHandler {
public:
	void pushEvent(EDAevent ev_) { events.push(ev_); }
	EDAevent getEvent(EDAevent ev_) { EDAevent ev = events.back(); events.pop(); return ev; }
	bool isEmpty() { return events.empty(); }
private:
	std::queue<EDAevent> events;
};
