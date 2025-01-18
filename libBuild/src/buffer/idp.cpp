#include "eutil/buffer/idp.hpp"

namespace util
{
    IDPool::IDPool()
        : m_minID(0), m_maxID(0)
    {}

    IDPool::IDPool(uint64_t minID, uint64_t maxID)
        : m_minID(minID), m_maxID(maxID)
    {
        for (uint64_t i = minID; i <= maxID; ++i)
        {
            idQueue.emplace_back(i);
        }
    }

    IDPool::~IDPool()
    {
        for(auto it = idList.end(); it != idList.begin(); --it)
        {
            releaseID(**it);
        }
    }

    ID IDPool::getID()
    {
        if (idQueue.empty())
        {
            return ID();
        }

        uint64_t idRaw = idQueue.front();
        idQueue.pop_front();

        auto id = std::make_shared<uint64_t>(idRaw);
        idList.emplace_back(id);
        return ID(id, this);
    }

    void IDPool::releaseID(uint64_t id)
    {
        for (auto it = idList.begin(); it != idList.end(); ++it)
        {
            if (**it == id)
            {
                idQueue.emplace_back(id);
                **it = -1;
                idList.erase(it);
                break;
            }
        }

        sortIDs();
    }

    void IDPool::releaseID(ID id)
    {
        releaseID(id.get());
    }

    uint64_t IDPool::getMinID() const
    {
        return m_minID;
    }

    uint64_t IDPool::getMaxID() const
    {
        return m_maxID;
    }

    uint64_t IDPool::getAvailableIDs() const
    {
        return idQueue.size();
    }

    void IDPool::sortIDs()
    {
        std::sort(idQueue.begin(), idQueue.end());
    }

    ID::ID()
        : id(nullptr), pool(nullptr)
    {
    }

    ID::ID(std::shared_ptr<uint64_t> id, IDPool* pool)
        : id(id), pool(pool)
    {
    }

    ID::ID(const ID& other)
        : id(other.id), pool(other.pool)
    {
    }

    ID::ID(ID&& other)
        : id(std::move(other.id)), pool(std::move(other.pool))
    {
    }

    ID::~ID()
    {
        if (id)
        {
            pool->releaseID(*id);
        }
    }

    uint64_t ID::get() const
    {
        return *id;
    }

    constexpr ID &ID::operator=(const ID &other) noexcept
    {
        id = other.id;
        pool = other.pool;
        return *this;
    }

    constexpr ID &ID::operator=(ID &&other) noexcept
    {
        id = std::move(other.id);
        pool = std::move(other.pool);
        return *this;
    }
}