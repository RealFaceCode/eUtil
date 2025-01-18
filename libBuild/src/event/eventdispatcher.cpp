#include "eutil/event/eventdispatcher.hpp"

namespace eutil
{
    EventDispatcher::EventDispatcher(Event& event)
        : event(event)
    {}

    EventDispatcher::EventDispatcher(Event* event)
        : event(*event)
    {}
}