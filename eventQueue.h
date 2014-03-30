#ifndef EVENTQ_H
#define EVENTQ_H
#include <string>
#include <vector>
#include "gameEvent.h"

class EventQueue {
public:
    static EventQueue& getInstance();
    void displayEvents() const;

    void push(GameEvent& g) { events.push_back(g); }
    const GameEvent& pop() { return events.pop_front(); }
    GameEvent& front() { return events.front(); }
    const GameEvent& front() const { return events.front(); }
    GameEvent& back() { return events.back(); }
    const GameEvent& back() const { return events.back(); }

    const int size() const { return events.size(); }
    const bool empty() const { return events.empty(); }
    void clear() { events.clear(); } 
    
    GameEvent& operator[](int i) { return events[i]; }
    const GameEvent& operator[](int i) const { return events[i]; }}

    std::vector<GameEvent&> findEventsByType(const std::string&); // return list of all matching events

private:
    EventQueue();
    EventQueue(const EventQueue&);
    EventQueue& operator=(const EventQueue&);

    std::vector<GameEvent&> events;
};
#endif
