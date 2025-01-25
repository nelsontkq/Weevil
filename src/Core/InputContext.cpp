//
// Created by nelson on 1/24/25.
//

#include "WeevilEngine/InputContext.h"

void wv::InputContext::bind_action(std::string_view action, SDL_Scancode key) {
  action_bindings_[std::string(action)].insert(key);
}

void wv::InputContext::unbind_action(std::string_view action) {
  assert(action_bindings_.find(std::string(action)) != action_bindings_.end());
  action_bindings_.erase(std::string(action));
}
void wv::InputContext::unbind_action(std::string_view action, SDL_Scancode key) {
  assert(action_bindings_.find(std::string(action)) != action_bindings_.end());
  auto &keys = action_bindings_[std::string(action)];
  assert(keys.find(key) != keys.end());
  keys.erase(key);
  if (keys.empty()) {
    action_bindings_.erase(std::string(action));
  }
}
bool wv::InputContext::is_triggered(std::string_view action) const {
  auto it = action_bindings_.find(std::string(action));
  if (it == action_bindings_.end()) {
    return false;
  }
  for (SDL_Scancode key : it->second) {
    if (triggered_.find(key) != triggered_.end()) {
      return true;
    }
  }
  return false;
}

void wv::InputContext::process_event(SDL_Event &ev) {
  switch (ev.type) {
    case SDL_EVENT_KEY_DOWN:
      if (!ev.key.repeat) {
        triggered_.insert(ev.key.scancode);
      }
      break;

    case SDL_EVENT_KEY_UP:
      triggered_.erase(ev.key.scancode);
      break;

    default:
      break;
  }
}
