#ifndef STRATEGY_H
#define STRATEGY_H
#include "listener.h"

enum strats {
    NONE,
    CHASE_STRAT,
    BULLET_STRAT
};

/***** Base Strategy class, don't instantiate */

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
    //virtual void update(Uint32 ticks) {};
    GridElement* getMyGE() const { return myGE; }
private:
    GridElement* myGE;
};

/******* Strategy Specializations *******/

class ChaseStrategy : public Strategy {
public:
       ChaseStrategy(GridElement*);
       virtual void init();
       virtual void registerListeners();
       ChaseStrategy* clone() const {
           return new ChaseStrategy(*this);
       }
       virtual void onMove(const GameEvents::Event*);
};
void chaseStratMoveForwarder(Listener*, const GameEvents::Event*);

class BulletStrategy : public Strategy {
public:
      BulletStrategy(GridElement*);
      virtual void init();
      virtual void registerListeners();
      //virtual void update(Uint32 ticks);
      BulletStrategy* clone() const {
          return new BulletStrategy(*this);
      }
      virtual void onCollide(const GameEvents::Event*);
};
void bulletStratCollideForwarder(Listener*, const GameEvents::Event*);
#endif
