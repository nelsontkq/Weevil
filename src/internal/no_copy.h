#pragma once

namespace wv {
struct NoCopy {
  NoCopy() = default;
  ~NoCopy() = default;
  NoCopy(const NoCopy&) = delete;
  NoCopy(NoCopy&&) = delete;
  auto operator=(const NoCopy&) -> NoCopy& = delete;
  auto operator=(NoCopy&&) -> NoCopy& = delete;
};
}  // namespace wv
