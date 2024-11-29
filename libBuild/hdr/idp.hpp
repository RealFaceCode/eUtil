#pragma once

#include "defines.hpp"
#include <queue>
#include <deque>
#include <memory>
#include <cstdint>

namespace eutil::idp
{
    class ID;

    class EUTIL_API IDPool
    {
    public:
        IDPool();
        IDPool(uint64_t minID, uint64_t maxID);
        ~IDPool();

        ID getID();
        void releaseID(uint64_t id);
        void releaseID(ID id);

        uint64_t getMinID() const;
        uint64_t getMaxID() const;
        uint64_t getAvailableIDs() const;

    private:
        void sortIDs();

        uint64_t m_minID;
        uint64_t m_maxID;
        std::deque<uint64_t> idQueue;
        std::vector<std::shared_ptr<uint64_t>> idList;
    };

    class EUTIL_API ID
    {
    public:
        ID();
        ID(std::shared_ptr<uint64_t> id, IDPool* pool);
        ID(const ID& other);
        ID(ID&& other);
        ~ID();

        uint64_t get() const;

        constexpr ID &operator=(const ID &) noexcept;
        constexpr ID &operator=(ID &&) noexcept;

    private:
        std::shared_ptr<uint64_t> id;
        IDPool* pool;
    };
}