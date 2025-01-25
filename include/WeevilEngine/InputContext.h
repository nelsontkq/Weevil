#pragma once

#include "wvpch.h"

namespace wv{
class InputContext : NoCopy {
 public:
  void bind_action(std::string_view action, SDL_Scancode key);

  void unbind_action(std::string_view action);
  void unbind_action(std::string_view action, SDL_Scancode key);

  bool is_triggered(std::string_view action) const;
  void process_event(SDL_Event &ev);

 private:
  std::unordered_map<std::string, std::unordered_set<SDL_Scancode>> action_bindings_;
  std::unordered_set<SDL_Scancode> triggered_;
};

}  // namespace wv
