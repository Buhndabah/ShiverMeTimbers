#include "gameEvents.h"

namespace GameEvents {

EventQueue& EventQueue::getInstance() {
    static EventQueue instance;
    return instance;
}

// Return all events matching type field
std::vector<Event> EventQueue::findEventsByType(int t) {
    std::vector<Event> retEvents;

    for(std::vector<Event>::const_iterator it=events.begin(); it!=events.end(); ++it)
    {
        if((*it).type==t)
        {
            retEvents.push_back(*it);
        }
    }
    return retEvents;
}

// Return all events with matching actor field
std::vector<Event> EventQueue::findEventsByActor(const std::string& name) {
    std::vector<Event> retEvents;

    for(std::vector<Event>::const_iterator it = events.begin(); it!=events.end(); ++it)
    {
        if((*it).actor.compare(name) == 0)
        {
            retEvents.push_back(*it);
        }
    }
    return retEvents;
}
}
