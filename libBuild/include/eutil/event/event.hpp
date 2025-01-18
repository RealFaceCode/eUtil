#pragma once

#include "eutil/defines.hpp"
#include <typeinfo>
#include <string>

namespace eutil
{
    struct EUTIL_API Event
    {
        Event() = default;
        virtual ~Event() = default;

        bool isHandled() const;
        void setHandled();

        virtual std::size_t getType()
        {
            if(name.empty())
                name = std::string(typeid(*this).name());
            if(type == 0)
                type = std::hash<std::string>{}(name);
            return type;
        }
        virtual std::string getName()
        {
            if(name.empty())
                name = std::string(typeid(*this).name());
            return name;
        }

    private:
        bool handled = false;
        std::string name;
        size_t type;
    };
}