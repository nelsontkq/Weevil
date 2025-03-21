#pragma once

#include <cstdint>

namespace wv {

class UUID {
 public:
  UUID();

  explicit UUID(uint64_t uuid);

  UUID(const UUID &other);

  operator uint64_t() const { return uuid_; }

 private:
  uint64_t uuid_;
};
}  // namespace wv
namespace std {
template <typename T>
struct hash;

template <>
struct hash<wv::UUID> {
  std::size_t operator()(const wv::UUID &uuid) const { return (uint64_t)uuid; }
};

}  // namespace std
