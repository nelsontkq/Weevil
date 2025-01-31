#pragma once
#include "Scene.h"
#include "wvpch.h"
namespace wv {

#pragma once
class SceneManager {
 public:
  Scene& init();

  void pop();

  [[nodiscard]] auto empty() const -> bool;

  Scene& current() const;

 private:
  std::vector<Scene&> scene_stack_;
};
;

}  // namespace wv
