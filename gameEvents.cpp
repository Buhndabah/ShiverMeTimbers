#include "gameEvents.h"

namespace GameEvents {

EventQueue& EventQueue::getInstance() {
    static EventQueue instance;
    return instance;
}

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
