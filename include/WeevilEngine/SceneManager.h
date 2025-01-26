#pragma once
#include "wvpch.h"
#include "Scene.h";
namespace wv{

#pragma once
class SceneManager {
 public:
  template<Derived<Scene> SceneType, typename... Args>
  void push(Args &&... args) {
    auto scene = std::make_unique<SceneType>(std::forward<Args>(args)...);
    scene->on_enter();
    scene_stack_.push_back(std::move(scene));
  }

  template<typename SceneType, typename... Args>
  void change(Args &&... args) {
    WV_ASSERT(!scene_stack_.empty(), "A scene is required to change scene!");
    scene_stack_.back()->on_exit();
    scene_stack_.pop_back();
    auto scene = std::make_unique<SceneType>(std::forward<Args>(args)...);
    scene->on_enter();
    scene_stack_.push_back(std::move(scene));
  }

  void pop();

  // Update the top of the stack.
  void update(float dt);

  bool empty() const;

 private:
  std::vector<std::unique_ptr<Scene>> scene_stack_;
};;

} // wv
