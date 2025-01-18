#ifndef WV_UUID_HPP
#define WV_UUID_HPP

#include <cstdint>
namespace wv
{

    class UUID
    {
    public:
        UUID();
        explicit UUID(uint64_t uuid);
        UUID(const UUID &other);

        operator uint64_t() const { return uuid_; }

    private:
        uint64_t uuid_;
    };
}

#endif // WV_UUID_HPP
