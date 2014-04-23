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
    HUDREMOVE_EVENT,
    ROUNDOVER_EVENT,
    CREATE_EVENT,
    SCORE_EVENT,
    WIN_EVENT,
    LOSE_EVENT,
    GAMEEND_EVENT,
    CLICK_EVENT,
    TYPES_MAX = 13
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
    DamageEvent(const std::string& from, const std::string& to, const Vector2f& pos, float amount) : 
        Event(DAMAGE_EVENT, from, pos),
        target(to),
        damage(amount)
    { }
    std::string getTarget() const { return target; }
    float getDamage() const { return damage; }
private:
    std::string target;
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

class HUDRemoveEvent : public Event {
public:
    HUDRemoveEvent(const std::string& from, const Vector2f& pos) :
        Event(HUDREMOVE_EVENT, from, pos)
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
    CreateEvent(const std::string& from, const std::string& n, const Vector2f& pos, int dir, Vector2f t, int st) :
        Event(CREATE_EVENT, from, pos),
        sprite(n),
        direction(dir),
	target(t),
        strat(st)
        { }
    std::string getSprite() const { return sprite; }
    int getDir() const { return direction; }
    Vector2f getTarget() const { return target; }
    int getStrat() const { return strat; }
private:
    std::string sprite;
    int direction;
    Vector2f target;
    int strat;
};

class ScoreEvent: public Event {
public:
    ScoreEvent(const std::string& from, const Vector2f& pos, int amount) : 
        Event(SCORE_EVENT, from, pos),
        score(amount)
        { }
    int getScore() const { return score; }
private:
    int score;
};

class WinEvent: public Event {
public:
    WinEvent(const std::string& from, const Vector2f& pos) :
        Event(WIN_EVENT, from, pos)
    { }
};

class LoseEvent: public Event {
public:
    LoseEvent(const std::string& from, const Vector2f& pos) :
        Event(LOSE_EVENT, from, pos)
    { }
};

class GameEndEvent : public Event {
public: 
    GameEndEvent(const std::string& from, const Vector2f& pos, bool r) :
        Event(GAMEEND_EVENT, from, pos),
        restart(r)
    { }
    bool getRestart() const { return restart; }
private:
    bool restart;
};

class ClickEvent: public Event {
public:
    ClickEvent(const std::string& from, const Vector2f& pos) :
        Event(CLICK_EVENT, from, pos)
    { }
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

    void reinit();
    void clearEvents();

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
