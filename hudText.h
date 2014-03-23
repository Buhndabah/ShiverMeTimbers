#ifndef HUDTEXT_H
#define HUDTEXT_H
#include "hudComponent.h"
#include "ioManager.h"

class HUDText: public HUDComponent {
public:
    // name, position, visible
    HUDText(const std::string& n, const Vector2f& p, bool v);
    // name, position, visible, text, centered(true/false)
    HUDText(const std::string& n, const Vector2f& p, bool b, const std::string s, bool c);
    HUDText(const HUDText& rhs);
    HUDText& operator=(const HUDText&);
    virtual ~HUDText() {}

    virtual void draw() const;
    virtual void update(Uint32);
    void setText(const std::string&);
private:
    IOManager& io;
    std::string text;
    bool centered;
};
#endif
