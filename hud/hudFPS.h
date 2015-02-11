#ifndef HUDFPS_H
#define HUDFPS_H
#include "hudComponent.h"
#include "clock.h"
#include "ioManager.h"

class HUDFPS : public HUDComponent {
public:
    HUDFPS(const std::string& n, const Vector2f& p, bool v);
    HUDFPS(const HUDFPS& rhs);
    HUDFPS& operator=(const HUDFPS&);
    virtual ~HUDFPS() {}

    virtual void draw() const;
    virtual void update(Uint32);
private:
    IOManager& io;
    Clock& clock;
};
#endif
