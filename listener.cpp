#include "listener.h"

Listener::Listener() :
    eq(GameEvents::EventQueue::getInstance())
{ }

Listener& Listener::operator=(const Listener& rhs) {
}
