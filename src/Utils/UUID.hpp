#pragma once

#include <cstdint>

class UUID {
public:
    UUID();
    explicit UUID(uint64_t uuid);
    UUID(const UUID& other);

    operator uint64_t() const { return uuid_; }

private:
    uint64_t uuid_;
};
