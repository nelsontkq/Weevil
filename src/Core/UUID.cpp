#include "WeevilEngine/UUID.h"
#include <random>

wv::UUID::UUID() {
  static std::random_device rd;
  static std::mt19937_64 generator(rd());
  static std::uniform_int_distribution<uint64_t> distribution;

  uuid_ = distribution(generator);
}

wv::UUID::UUID(uint64_t uuid)
    : uuid_(uuid) {}

wv::UUID::UUID(const UUID &other)
    : uuid_(other.uuid_) {}
