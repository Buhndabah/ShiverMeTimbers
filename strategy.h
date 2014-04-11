#ifndef STRATEGY_H
#define STRATEGY_H
#include "listener.h"

enum strats {
    NONE,
    CHASE_STRAT,
    BULLET_STRAT
};

class GridElement;
class Strategy : public Listener{
public:
    Strategy(GridElement* g) : myGE(g) {}
    Strategy(const Strategy& rhs) : myGE(rhs.myGE) {}
    Strategy& operator=(const Strategy& rhs) { 
        if(this == &rhs) return *this;
        myGE = rhs.myGE;  
        return *this;
    }
    virtual void init()=0;
    virtual void registerListeners()=0;
    virtual Strategy* clone() const=0;
    GridElement* getMyGE() const { return myGE; }
private:
    GridElement* myGE;
};

class ChaseStrategy : public Strategy {
public:
       ChaseStrategy(GridElement*);
       virtual void init();
       virtual void registerListeners();
       ChaseStrategy* clone() const {
           return new ChaseStrategy(*this);
       }
       virtual void onMove(const GameEvents::Event);
};
void chaseStratMoveForwarder(Listener*, const GameEvents::Event);
#endif
