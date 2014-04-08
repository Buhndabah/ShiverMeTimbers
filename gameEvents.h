#ifndef EVENTQ_H
#define EVENTQ_H
#include <string>
#include <list>
#include <vector>
#include "vector2f.h"

namespace GameEvents {

enum types {
    DAMAGE_EVENT,
    MOVE_EVENT,
    COLLIDE_EVENT,
    DEATH_EVENT
};

struct Event {
    int type;
    std::string actor;

    std::list<float> data;
    Vector2f location;
    Vector2f direction;

    Event() : 
        type(),
        actor(),
        data(),
        location(),
        direction()
    { }
};

class EventQueue {
public:
    static EventQueue& getInstance();
    void displayEvents() const;

    void push(const Event& e) { events.push_back(e); }

    int size() const { return events.size(); }
    bool empty() const { return events.empty(); }
    void clear() { events.clear(); }

    Event operator[](int i) { return events[i]; }
    const Event operator[](int i) const { return events[i]; }

    std::vector<Event> findEventsByType(int t);
    std::vector<Event> findEventsByActor(const std::string& name);

private:
    EventQueue() : events() { };
    EventQueue(const EventQueue&);
    EventQueue& operator=(const EventQueue&);

    std::vector<Event> events;
};

}
#endif
