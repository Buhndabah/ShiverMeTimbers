#ifndef HUDSCORE_H
#define HUDSCORE_H

#include "ioManager.h"
#include "hudComponent.h"

class HUDScore : public HUDComponent {
public:
    HUDScore(const std::string& n, const Vector2f& pos, bool vis);
    HUDScore(const HUDScore& rhs);

    virtual void draw() const;
    virtual void update(Uint32);

    virtual void onScore(const GameEvents::ScoreEvent* e);

    int getScore() const { return score; }
private:
    virtual void registerListeners();
    const IOManager& io;
    int score;
    int limit;
};

void HUDScoreScoreForwarder(Listener*, const GameEvents::Event*);
#endif
