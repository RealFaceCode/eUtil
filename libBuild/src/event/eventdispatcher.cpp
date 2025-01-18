#include "eutil/event/eventdispatcher.hpp"

namespace util
{
    EventDispatcher::EventDispatcher(Event& event)
        : event(event)
    {}

    EventDispatcher::EventDispatcher(Event* event)
        : event(*event)
    {}
}