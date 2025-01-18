#include "eutil/event/event.hpp"

namespace util
{
    bool Event::isHandled() const
    {
        return handled;
    }

    void Event::setHandled()
    {
        handled = true;
    }
}