#include "eutil/event/event.hpp"

namespace eutil
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