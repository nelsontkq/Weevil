#include "UUID.hpp"
#include <random>

using namespace wv;

UUID::UUID()
{
    static std::random_device rd;
    static std::mt19937_64 generator(rd());
    static std::uniform_int_distribution<uint64_t> distribution;

    uuid_ = distribution(generator);
}

UUID::UUID(uint64_t uuid)
    : uuid_(uuid) {}

UUID::UUID(const UUID &other)
    : uuid_(other.uuid_) {}