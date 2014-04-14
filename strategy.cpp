#include "strategy.h"
#include "gridElement.h"

ChaseStrategy::ChaseStrategy(GridElement* g) :
    Strategy(g)
{ 
    init();    
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
    if(e->getSource().compare("coolyeti")==0)
    {
        // Within tolerance of destination
        if(dist <= radius)
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

BulletStrategy::BulletStrategy(GridElement* g) :
    Strategy(g)
{
    init();
}

void BulletStrategy::init() {
    registerListeners();
}

void BulletStrategy::onCollide(const GameEvents::Event* e)
{
    if(e->getSource().compare(getMyGE()->getName())) return; // Ignore collisions with our creator
    GameEvents::EventQueue::getInstance().push(new GameEvents::DamageEvent(e->getSource(), getMyGE()->getPosition(), 10));
    GameEvents::EventQueue::getInstance().push(new GameEvents::DeathEvent(getMyGE()->getName(), getMyGE()->getPosition()));
}

void bulletStratCollideForwarder(Listener* context, const GameEvents::Event *e) {
    dynamic_cast<BulletStrategy*>(context)->onCollide(e);
}

void BulletStrategy::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::COLLIDE_EVENT, static_cast<Listener*>(this), &bulletStratCollideForwarder);
}
