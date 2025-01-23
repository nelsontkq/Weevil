
#ifndef WEEVIL_INCLUDE_WEEVILENGINE_NOCOPY_H_
#define WEEVIL_INCLUDE_WEEVILENGINE_NOCOPY_H_

namespace wv {
struct NoCopy {
  NoCopy() = default;
  NoCopy(const NoCopy&) = delete;
  NoCopy(NoCopy&&) = delete;
  NoCopy& operator=(const NoCopy&) = delete;
  NoCopy& operator=(NoCopy&&) = delete;
};
}  // namespace wv

#endif  // WEEVIL_INCLUDE_WEEVILENGINE_NOCOPY_H_