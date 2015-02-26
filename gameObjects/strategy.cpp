#include <stdlib.h>
#include "strategy.h"
#include "gridElement.h"
#include <cmath>

ChaseStrategy::ChaseStrategy(GridElement* g, GridElement* c) :
    Strategy(g, CHASE_STRAT),
    chaseTarget(c)
{ 
    init();    
}

ChaseStrategy::ChaseStrategy(const ChaseStrategy& rhs) :
    Strategy(rhs),
    chaseTarget(rhs.chaseTarget)
{
    init();
}

ChaseStrategy& ChaseStrategy::operator=(const ChaseStrategy& rhs) {
    Strategy::operator=(rhs);
    // don't delete gridelement since it lives in mapmanager
    if(this == &rhs) return *this;

    chaseTarget = rhs.chaseTarget;
    return *this;
}

void ChaseStrategy::init() {
    registerListeners();
}

void chaseStratMoveForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<ChaseStrategy*>(context)->onMove(e);
}

void ChaseStrategy::onMove(const GameEvents::Event *e) {
    // radius around midpoint of this sprite
    float radius = sqrt(pow(getMyGE()->getSprite().getW(),2)+pow(getMyGE()->getSprite().getH(),2))/2; 
    // distance between midpoint of this sprite and that sprite
    float dist = sqrt(pow(e->getPosition()[0] - getMyGE()->getPosition()[0],2) + pow(e->getPosition()[1] - getMyGE()->getPosition()[1],2));
    // to the left to the left
    bool left   =  ((e->getPosition()[0] > getMyGE()->getPosition()[0])  ? true : false);
    // to the right
    bool right  =  ((e->getPosition()[0] <= getMyGE()->getPosition()[0])   ? true : false);
    // above
    bool up     =  ((e->getPosition()[1] > getMyGE()->getPosition()[1])  ? true : false);
    // below
    bool down   =  ((e->getPosition()[1] <= getMyGE()->getPosition()[1])   ? true : false);
    // Their location is within the sprite's left and right edges
    bool inW    = (e->getPosition()[0] > getMyGE()->getPosition()[0]) && 
                  (e->getPosition()[0] < (getMyGE()->getPosition()[0]  + getMyGE()->getSprite().getW()));
    // Their location is within the sprite's bottom and top edges
    bool inH    = (e->getPosition()[1] > getMyGE()->getPosition()[1]) && 
                  (e->getPosition()[1] < (getMyGE()->getPosition()[1]  + getMyGE()->getSprite().getH()));
    //std::cerr<< "source is " << e->getSource() << " my name is " << getMyGE()->getName() << std::endl;
    if(e->getSource().compare(chaseTarget->getName())==0)
    {
        // Within tolerance of destination
        if(dist <= radius-2)
            getMyGE()->stop();
        else if(inW)
        {
            if(down)
            {
                getMyGE()->moveUp();
            }
            else if(up)
            {
                getMyGE()->moveDown();
            }
        }
        else if(inH)
        {
            if(left)
            {
                getMyGE()->moveRight();
            }
            else if(right)
            {
                getMyGE()->moveLeft();
            }
        }
        else if(up&&left)
        {
            getMyGE()->moveDownRight();
        }
        else if(up&&right)
        {
            getMyGE()->moveDownLeft();
        }
        else if(down&&left)
        {
            getMyGE()->moveUpRight();
        }
        else if(down&&right)
        {
            getMyGE()->moveUpLeft();
        }
    }
}

void ChaseStrategy::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::MOVE_EVENT, static_cast<Listener*>(this), &chaseStratMoveForwarder);
}

/*********** Bullet Strategy ****************/

BulletStrategy::BulletStrategy(GridElement* g, int dir) :
    Strategy(g, BULLET_STRAT),
    direction(dir),
    vDir(Vector2f(0,0))
{
    init();
    switch(dir) {
        case UP:
            g->moveUp();
            break;
        case UPLEFT:
            g->moveUpLeft();
            break;
        case UPRIGHT:
            g->moveUpRight();
            break;
        case DOWN:
            g->moveDown();
            break;
        case DOWNLEFT: 
            g->moveDownLeft();
            break;
        case DOWNRIGHT:
            g->moveDownRight();
            break;
        case LEFT:
            g->moveLeft();
            break;
        case RIGHT:
            g->moveRight();
            break;
	case -1:
	    std::cerr << "why is this being called?" << std::endl;
	    break;
        default:
            std::cerr<< "Dir " << dir << std::endl;
            throw std::string("Tried to create bullet with invalid direction ");
            break;
    }
}

BulletStrategy::BulletStrategy(GridElement* g, Vector2f target) :
    Strategy(g, BULLET_STRAT),
    direction(-1),
    vDir(0,0)
{
    init();
    std::cerr << "THIS should be being called" << std::endl;
    g->moveTowards(target);
}

void BulletStrategy::init() {
    registerListeners();
}

void BulletStrategy::onCollide(const GameEvents::Event* e)
{
    GridElement* g = getMyGE();
    const GameEvents::CollideEvent *c = dynamic_cast<const GameEvents::CollideEvent*>(e);
    std::string myName = g->getName();
    std::string to;

    // If this doesn't involve us, ignore it
    if(c->getSource().compare(myName) != 0 && c->getSubject().compare(myName) !=0) return;

    /* Get name of what we hit */
    /* Account for both them hitting us and us hitting them */
    if(c->getSource().compare(myName)==0)
        to = c->getSubject();
    else
        to = c->getSource();
    // ignore collisions with other bullets
    if(to.substr(to.length()-std::string("bullet").length()).compare("bullet")==0) {
        return;
    }
    // ignore collisions with our source
    if(source.compare(to)==0)
    {
        return;
    } 
    /* Push both a collide event and a death notification for ourself */
    GameEvents::EventQueue::getInstance().push(new GameEvents::DamageEvent(myName, to, g->getPosition(), 10));
    GameEvents::EventQueue::getInstance().push(new GameEvents::DeathEvent(myName, g->getPosition()));

}

/****** Listener registration stuff ******/

void bulletStratCollideForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<BulletStrategy*>(context)->onCollide(e);
}

void BulletStrategy::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::COLLIDE_EVENT, static_cast<Listener*>(this), &bulletStratCollideForwarder);
}

/********* Turret Strategy ***********************/

TurretStrategy::TurretStrategy(GridElement* g, GridElement* t) :
    Strategy(g, TURRET_STRAT),
    target(t),
    clock(Clock::getInstance()),
    timeBetweenShots(2000),
    ticks(clock.getTicks())
{
    init();
}

TurretStrategy::TurretStrategy(const TurretStrategy& rhs) :
    Strategy(rhs),
    target(rhs.target),
    clock(Clock::getInstance()),
    timeBetweenShots(rhs.timeBetweenShots),
    ticks(rhs.ticks)
{
    init();
}

TurretStrategy& TurretStrategy::operator=(const TurretStrategy& rhs) {
    Strategy::operator=(rhs);

    if(this == &rhs) return *this;

    target = rhs.target;
    timeBetweenShots = rhs.timeBetweenShots;
    ticks = rhs.ticks;
    return *this;
}

TurretStrategy::~TurretStrategy() {
    GameEvents::EventQueue::getInstance().push(new GameEvents::ScoreEvent("turret",getMyGE()->getPosition(),10));
    GameEvents::EventQueue::getInstance().push(new GameEvents::CreateEvent("turret", "icecream", getMyGE()->getPosition(), 0, Vector2f(0,0), TURRET_STRAT, 200));
}

void TurretStrategy::init() {
    registerListeners();
}

void turretStratMoveForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<TurretStrategy*>(context)->onMove(e);
}

void TurretStrategy::onMove(const GameEvents::Event* e) {

    if(isSuppressed() ) return;

    // ignore events not from our target
    if(e->getSource().compare(target->getName())!=0) { return; }
    
    if(clock.getTicks()-ticks < timeBetweenShots) return;
    else ticks = clock.getTicks();

    bool left = ((e->getPosition()[0] < getMyGE()->getPosition()[0]));
    bool right = ((e->getPosition()[0] >= getMyGE()->getPosition()[0]));
    bool up = ((e->getPosition()[1] < getMyGE()->getPosition()[1]));
    bool down = ((e->getPosition()[1] > getMyGE()->getPosition()[1]));
    bool inW    = (e->getPosition()[0] > getMyGE()->getPosition()[0]) &&
                (e->getPosition()[0] < (getMyGE()->getPosition()[0]  + getMyGE()->getSprite().getW()));
    // Their location is within the sprite's bottom and top edges
    bool inH    = (e->getPosition()[1] > getMyGE()->getPosition()[1]) &&
                  (e->getPosition()[1] < (getMyGE()->getPosition()[1]  + getMyGE()->getSprite().getH()));
    // If this is the thing we're shooting
    if(e->getSource().compare(target->getName())==0)
    {
       if(inW)
       {
           if(down)
           {
               getMyGE()->shoot(DOWN);
           }
           else if(up)
           {
               getMyGE()->shoot(UP);
           }
       }
       else if(inH)
       {
           if(left)
           {
               getMyGE()->shoot(LEFT);
           }
           else if(right)
           {
               getMyGE()->shoot(RIGHT);
           }
       }
       else if(up&&left)
       {
           getMyGE()->shoot(UPLEFT);
       }
       else if(up&&right)
       {
           getMyGE()->shoot(UPRIGHT);
       }
       else if(down&&left)
       {
           getMyGE()->shoot(DOWNRIGHT);
       }
       else if(down&&right)
       {
           getMyGE()->shoot(DOWNLEFT);
       }
    }
}

void TurretStrategy::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::MOVE_EVENT, static_cast<Listener*>(this), &turretStratMoveForwarder);
}
