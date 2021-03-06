#include "gameEvents.h"

namespace GameEvents {

EventQueue& EventQueue::getInstance() {
    static EventQueue instance;
    return instance;
}

EventQueue::~EventQueue() {
    clearEvents();
}

void EventQueue::reinit() {
    clearEvents();
    listeners.clear();
}

void EventQueue::clearEvents() {
    while(!incoming.empty())
    {
        delete incoming.front();
        incoming.erase(incoming.begin());
    }
    while(!outgoing.empty())
    {
        delete outgoing.front();
        outgoing.erase(outgoing.begin());
    }
}

void EventQueue::prepEvents() {
    if(!incoming.empty()) {
        outgoing = std::list<Event*>(incoming);
        incoming.clear();
        notify();
    }
}

// Return all events matching type field
std::list<Event*> EventQueue::findEventsByType(int t) {
    std::list<Event*> retEvents;

    for(std::list<Event*>::const_iterator it=outgoing.begin(); it!=outgoing.end(); ++it)
    {
        if((*it)->getType()==t)
        {
            retEvents.push_back(*it);
        }
    }
    return retEvents;
}

// Return all events with matching actor field
std::list<Event*> EventQueue::findEventsByActor(const std::string& name) {
    std::list<Event*> retEvents;

    for(std::list<Event*>::const_iterator it = outgoing.begin(); it!=outgoing.end(); ++it)
    {
        if((*it)->getSource().compare(name) == 0)
        {
            retEvents.push_back(*it);
        }
    }
    return retEvents;
}

// Add a listener l to event eventType, calling function fn
void EventQueue::addListener(types eventType, Listener* l, void(*fn)(Listener*, const Event*)) {
    listeners[eventType].push_back(std::pair<Listener*, void(*)(Listener*, const Event*)>(l, fn));
}

// remove listener from list
void EventQueue::removeListener(Listener* l) {
    for(std::map<int, std::list<std::pair<Listener*, void(*)(Listener*, const Event*)> > >::iterator it = listeners.begin(); it!= listeners.end(); ++it)
    {
        std::list<std::pair<Listener*, void(*)(Listener*, const Event*)> >::iterator it2 = (*it).second.begin();
        while(it2 != ((*it).second.end()))
        {
            if((*it2).first == l)
            {
                (*it).second.erase(it2++);
            }
            else
            {
                it2++;
            }
        }
    }
}

// for each event, notify each listener that cares, calling its callback
void EventQueue::notify() {
    for(std::list<Event*>::const_iterator it=outgoing.begin(); it != outgoing.end(); ++it)
    {
        for(std::list<std::pair<Listener*, void(*)(Listener*, const Event*)> >::const_iterator it2 = listeners[(*it)->getType()].begin(); it2 != listeners[(*it)->getType()].end(); ++it2)
        {
            (*it2).second((*it2).first, (*it));
        }
    }
    while(!outgoing.empty()) {
        delete outgoing.front();
        outgoing.erase(outgoing.begin());
    }
}
}

