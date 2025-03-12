#ifndef EVENT_H
#define EVENT_H
#include "wire.h"

struct Event
{
    uint64_t time;
    Wire* wire;
    char state;
};

struct EventLess {
    bool operator()(Event* e1, Event* e2) const {
        return e1->time > e2->time;  // smaller time should come first and will point to accessors
    }
};

#endif
