#ifndef EVENTQ_H
#define EVENTQ_H
#include <string>
#include <list>
#include <map>
#include "vector2f.h"
#include "listener.h"

class Listener;
namespace GameEvents {

// Type definitions for Event
enum types {
    DAMAGE_EVENT,
    MOVE_EVENT,
    COLLIDE_EVENT,
    DEATH_EVENT,
    ROUNDOVER_EVENT,
    TYPES_MAX = 5
};

// Structure for holding data about event
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

// Maintains list of events, also has search functions
class EventQueue {
public:
    static EventQueue& getInstance();

    void prepEvents();
    void push(const Event& e) { incoming.push_back(e); }

    int sizeIn() const { return incoming.size(); }
    int sizeOut() const { return outgoing.size(); }
    bool emptyIn() const { return incoming.empty(); }
    bool emptyOut() const { return outgoing.empty(); }
    void clear() { outgoing.clear(); }


    /*Event operator[](int i) { return incoming[i]; }
    const Event operator[](int i) const { return incoming[i]; }*/

    std::list<Event> findEventsByType(int t);
    std::list<Event> findEventsByActor(const std::string& name);
    void addListener(types, Listener*, void (*)(Listener*, const Event));

private:
    EventQueue() : incoming(), outgoing(), listeners() {  
        for(int i = 0; i < TYPES_MAX; i++) 
            listeners.insert(std::pair<int, std::list<std::pair<Listener*, void(*)(Listener*, const Event)> > >(i, std::list<std::pair<Listener*, void(*)(Listener*, const Event)> >()));
    };
    EventQueue(const EventQueue&);
    EventQueue& operator=(const EventQueue&);

    void notify();

    std::list<Event> incoming;
    std::list<Event> outgoing;
    std::map<int, std::list<std::pair<Listener*, void (*)(Listener*, const Event)> > > listeners;
};

}
#endif
