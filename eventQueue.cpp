/*#include "eventQueue.h"

EventQueue& EventQueue::getInstance() {
    static EventQueue instance;
    return instance;
}

EventQueue::EventQueue() :
    events()
{ }

std::vector<GameEvent&> findEventsByType(const std::string& t) {
    std::vector<GameEvent&> retEvents;

    for(std::vector<GameEvent&>::const_iterator it = events.begin(); it!=events.end(); ++it)
    {
        if((*it).type().compare(t)==0)
        {
            retEvents.push_back(*it);
            events.erase(it);
        }
    }
    return retEvents;
}*/
