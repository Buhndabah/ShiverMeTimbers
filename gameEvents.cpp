#include "gameEvents.h"

namespace GameEvents {

EventQueue& EventQueue::getInstance() {
    static EventQueue instance;
    return instance;
}

void EventQueue::prepEvents() {
    if(!incoming.empty()) {
        outgoing = std::list<Event>(incoming);
        incoming.clear();
        notify();
        outgoing.clear();
    }
}

// Return all events matching type field
std::list<Event> EventQueue::findEventsByType(int t) {
    std::list<Event> retEvents;

    for(std::list<Event>::const_iterator it=outgoing.begin(); it!=outgoing.end(); ++it)
    {
        if((*it).type==t)
        {
            retEvents.push_back(*it);
        }
    }
    return retEvents;
}

// Return all events with matching actor field
std::list<Event> EventQueue::findEventsByActor(const std::string& name) {
    std::list<Event> retEvents;

    for(std::list<Event>::const_iterator it = outgoing.begin(); it!=outgoing.end(); ++it)
    {
        if((*it).actor.compare(name) == 0)
        {
            retEvents.push_back(*it);
        }
    }
    return retEvents;
}

// Add a listener l to event eventType, calling function fn
void EventQueue::addListener(types eventType, Listener* l, void(*fn)(Listener*, const Event)) {
    listeners[eventType].push_back(std::pair<Listener*, void(*)(Listener*, const Event)>(l, fn));
}

// for each event, notify each listener that cares, calling its callback
void EventQueue::notify() {
    for(std::list<Event>::const_iterator it=outgoing.begin(); it != outgoing.end(); ++it)
    {
        for(std::list<std::pair<Listener*, void(*)(Listener*, const Event)> >::const_iterator it2 = listeners[(*it).type].begin(); it2 != listeners[(*it).type].end(); ++it2)
        {
            (*it2).second((*it2).first, (*it));
        }
    }
}
}

