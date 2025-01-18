#pragma once

#include "eutil/defines.hpp"
#include "eutil/event/event.hpp"
#include <functional>

namespace util
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

            if (event.getType() == typeid(T).hash_code())
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