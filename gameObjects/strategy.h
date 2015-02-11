#ifndef STRATEGY_H
#define STRATEGY_H
#include "clock.h"
#include "listener.h"

enum strats {
    NONE,
    CHASE_STRAT,
    BULLET_STRAT,
    TURRET_STRAT
};

/***** Base Strategy class, don't instantiate */

class GridElement;
class Strategy : public Listener{
public:
    Strategy(GridElement* g, strats t) : suppressed(false), myGE(g), type(t) {}
    Strategy(const Strategy& rhs) : suppressed(rhs.suppressed), myGE(rhs.myGE), type(rhs.type){}
    Strategy& operator=(const Strategy& rhs) { 
        if(this == &rhs) return *this;
        suppressed = rhs.suppressed;
        myGE = rhs.myGE;  
        type = rhs.type;
        return *this;
    }
    virtual ~Strategy() {}
    virtual void suppress() { suppressed = true; };
    virtual void unsuppress() { suppressed = false; }
    virtual bool isSuppressed() const { return suppressed; }
    virtual void init()=0;
    virtual void registerListeners()=0;
    virtual Strategy* clone() const=0;
    //virtual void update(Uint32 ticks) {};
    GridElement* getMyGE() const { return myGE; }
    strats getType() const { return type; }
private:
    bool suppressed;
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
      BulletStrategy(GridElement*, Vector2f mouseDir);
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
    Vector2f vDir;
    std::string source;
};
void bulletStratCollideForwarder(Listener*, const GameEvents::Event*);

class TurretStrategy : public Strategy {
public:
    TurretStrategy(GridElement*, GridElement*);
    TurretStrategy(const TurretStrategy&);
    TurretStrategy& operator=(const TurretStrategy&);
    ~TurretStrategy();

    virtual void init();
    virtual void registerListeners();
   
    TurretStrategy* clone() const {
        return new TurretStrategy(*this);
    }

    virtual void onMove(const GameEvents::Event*);
private:
    GridElement* target;
    const Clock& clock;
    int timeBetweenShots;
    int ticks;
};
#endif
