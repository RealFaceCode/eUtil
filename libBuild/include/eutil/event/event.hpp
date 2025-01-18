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

        virtual size_t getType()
        {
            return typeid(*this).hash_code();
        }

        virtual std::string getName()
        {
            return typeid(*this).name();
        }

    private:
        bool handled = false;
        std::string name;
        size_t type;
    };
}