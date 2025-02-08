#include <weevil/core/uuid.h>

#include <weevil/pch.h>

wv::UUID::UUID() {
  static std::random_device rd;
  static std::mt19937_64 generator(rd());
  static std::uniform_int_distribution<uint64_t> distribution;

  uuid_ = distribution(generator);
}

wv::UUID::UUID(const uint64_t uuid) : uuid_(uuid) {}

wv::UUID::UUID(const UUID &other) = default;
