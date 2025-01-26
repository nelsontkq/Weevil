//
// Created by nelson on 1/25/25.
//

#include "WeevilEngine/SceneManager.h"

namespace wv{

void SceneManager::pop() {
  WV_ASSERT(!scene_stack_.empty(), "Must have a scene to pop it!");
  scene_stack_.back()->on_exit();
  scene_stack_.pop_back();
}

// Update the top of the stack.
void SceneManager::update(float dt) {
  WV_ASSERT(!scene_stack_.empty(), "A scene is required!");
  scene_stack_.back()->update(dt);
}

bool SceneManager::empty() const { return scene_stack_.empty(); }
} // wv