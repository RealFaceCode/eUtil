#pragma once

#include "eutil/defines.hpp"
#include "eutil/event/event.hpp"
#include <functional>

namespace eutil
{
    class EUTIL_API EventDispatcher
    {
        template <typename T>
        using EventCallback = std::function<bool(T&)>;
    public:

        explicit EventDispatcher(Event& event);
        explicit EventDispatcher(Event* event);
        ~EventDispatcher() = default;

        template <typename T>
        bool dispatch(EventCallback<T> callback)
        {
            if (event.isHandled())
                return false;

            if (event.getType() == std::hash<std::string>{}(typeid(T).name()))
            {
                if(callback(*(T*)&event))
                    event.setHandled();
                return true;
            }

            return false;
        }

        private:
            Event& event;
    };
}