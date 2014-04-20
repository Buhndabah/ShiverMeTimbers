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
    DAMAGERECEIVED_EVENT,
    MOVE_EVENT,
    COLLIDE_EVENT,
    DEATH_EVENT,
    ROUNDOVER_EVENT,
    CREATE_EVENT,
    TYPES_MAX = 7
};

// Base event class, abstract
class Event {
public:
    virtual ~Event() {}
    int getType() const { return type; }
    std::string getSource() const { return source; }
    Vector2f getPosition() const { return position; }
protected:
    Event() :
        type(),
        source(),
        position()
    { }
    Event(types t, const std::string& s, const Vector2f& p) : 
        type(t),
        source(s),
        position(p)
    { }
private:
    int type;
    std::string source;
    Vector2f position;
};

/******* Begin Event Specializations *******/

class DamageEvent : public Event {
public:
    DamageEvent(const std::string& to, const Vector2f& pos, float amount) : 
        Event(DAMAGE_EVENT, to, pos),
        damage(amount)
    { }
    float getDamage() const { return damage; }
private:
    float damage;
};

class DamageReceivedEvent : public Event {
public:
    DamageReceivedEvent(const std::string& name, const Vector2f& pos, float amount) :
        Event(DAMAGERECEIVED_EVENT, name, pos),
        damage(amount)
    { }
    float getDamage() const {return damage;}
private:
    float damage;
};

class MoveEvent : public Event {
public:
    MoveEvent(const std::string& from, const Vector2f& pos, const Vector2f& dir) :
        Event(MOVE_EVENT, from, pos),
        direction(dir)
    { }
    Vector2f getDir() const { return direction; }
private:
    Vector2f direction;
};

class CollideEvent : public Event {
public:
    CollideEvent(const std::string& from, const std::string& to, const Vector2f& pos) :
        Event(COLLIDE_EVENT, from, pos),
        subject(to)
    { }
    std::string getSubject() const { return subject; }
private: 
    std::string subject;
};

class DeathEvent : public Event {
public:
    DeathEvent(const std::string& from, const Vector2f& pos) :
        Event(DEATH_EVENT, from, pos)
    { }
};

class RoundOverEvent : public Event {
public:
    RoundOverEvent(const std::string& from) :
        Event(ROUNDOVER_EVENT, from, Vector2f(0,0))
    { }
};

class CreateEvent : public Event {
public:
    CreateEvent(const std::string& from, const std::string& n, const Vector2f& pos, int dir, int st) :
        Event(CREATE_EVENT, from, pos),
        sprite(n),
        direction(dir),
        strat(st)
        { }
    std::string getSprite() const { return sprite; }
    int getDir() const { return direction; }
    int getStrat() const { return strat; }
private:
    std::string sprite;
    int direction;
    int strat;
};

/***************** End Event Specializations ********************/

// Maintains list of events, also has search functions
class EventQueue {
public:
    static EventQueue& getInstance();
    ~EventQueue();

    void prepEvents();
    void push(Event* e) {incoming.push_back(e); }

    int sizeIn() const { return incoming.size(); }
    int sizeOut() const { return outgoing.size(); }
    bool emptyIn() const { return incoming.empty(); }
    bool emptyOut() const { return outgoing.empty(); }
    void clear() { outgoing.clear(); }


    /*Event operator[](int i) { return incoming[i]; }
    const Event operator[](int i) const { return incoming[i]; }*/

    std::list<Event*> findEventsByType(int t);
    std::list<Event*> findEventsByActor(const std::string& name);
    void addListener(types, Listener*, void (*)(Listener*, const Event*));
    void removeListener(Listener*);
private:
    EventQueue() : incoming(), outgoing(), listeners() {  
        for(int i = 0; i < TYPES_MAX; i++) 
            listeners.insert(std::pair<int, std::list<std::pair<Listener*, void(*)(Listener*, const Event*)> > >(i, std::list<std::pair<Listener*, void(*)(Listener*, const Event*)> >()));
    };
    EventQueue(const EventQueue&);
    EventQueue& operator=(const EventQueue&);

    void notify();

    std::list<Event*> incoming;
    std::list<Event*> outgoing;
    std::map<int, std::list<std::pair<Listener*, void (*)(Listener*, const Event*)> > > listeners;
};

}
#endif
