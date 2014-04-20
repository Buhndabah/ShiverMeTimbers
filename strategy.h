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
    Strategy(GridElement* g, strats t) : myGE(g), type(t) {}
    Strategy(const Strategy& rhs) : myGE(rhs.myGE), type(rhs.type){}
    Strategy& operator=(const Strategy& rhs) { 
        if(this == &rhs) return *this;
        myGE = rhs.myGE;  
        type = rhs.type;
        return *this;
    }
    virtual ~Strategy() {}
    virtual void init()=0;
    virtual void registerListeners()=0;
    virtual Strategy* clone() const=0;
    //virtual void update(Uint32 ticks) {};
    GridElement* getMyGE() const { return myGE; }
    strats getType() const { return type; }
private:
    GridElement* myGE;
    strats type;
};

/******* Strategy Specializations *******/

class ChaseStrategy : public Strategy {
public:
       ChaseStrategy(GridElement*,GridElement*);
       ChaseStrategy(const ChaseStrategy&);
       ChaseStrategy& operator=(const ChaseStrategy& rhs);
       virtual ~ChaseStrategy() {}
       virtual void init();
       virtual void registerListeners();
       ChaseStrategy* clone() const {
           return new ChaseStrategy(*this);
       }
       virtual void onMove(const GameEvents::Event*);
private:
    GridElement* chaseTarget;
};
void chaseStratMoveForwarder(Listener*, const GameEvents::Event*);

class BulletStrategy : public Strategy {
public:
      BulletStrategy(GridElement*, int dir);
      virtual void init();
      virtual void registerListeners();
      //virtual void update(Uint32 ticks);
      BulletStrategy* clone() const {
          return new BulletStrategy(*this);
      }
      virtual void onCollide(const GameEvents::Event*);
      void setSource(const std::string& s) { source = s; }
      std::string getSource() const { return source; }
private:
    int direction;
    std::string source;
};
void bulletStratCollideForwarder(Listener*, const GameEvents::Event*);
#endif
