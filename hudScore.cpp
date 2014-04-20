#include "hudScore.h"

HUDScore::HUDScore(const std::string& n, const Vector2f& pos, bool vis) :
    HUDComponent(n, pos, vis),
    io(IOManager::getInstance()),
    score(0)
{ 
    registerListeners();
}

HUDScore::HUDScore(const HUDScore& rhs) :
    HUDComponent(rhs),
    io(IOManager::getInstance()),
    score(rhs.score)
{
    registerListeners();
}

void HUDScore::draw() const {
    
    if(isVisible())
    {
        io.printMessageValueAt("Score: ", score, getPosition()[0], getPosition()[1]);
    }
}

void HUDScore::update(Uint32 ticks) {
    (void)ticks;
}

void HUDScore::onScore(const GameEvents::ScoreEvent* e) {
    score += e->getScore();
}

void HUDScore::registerListeners() {
    GameEvents::EventQueue::getInstance().addListener(GameEvents::SCORE_EVENT, static_cast<Listener*>(this), &HUDScoreScoreForwarder);
}

void HUDScoreScoreForwarder(Listener* context, const GameEvents::Event* e) {
    dynamic_cast<HUDScore*>(context)->onScore(dynamic_cast<const GameEvents::ScoreEvent*>(e));
}

