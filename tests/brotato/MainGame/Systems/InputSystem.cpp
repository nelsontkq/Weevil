//
// Created by nelson on 1/24/25.
//

#include "InputSystem.h"

#include "../BrotatoComponents.h"
constexpr std::string_view WALK_LEFT = "left";
constexpr std::string_view WALK_DOWN = "down";
constexpr std::string_view WALK_RIGHT = "right";
constexpr std::string_view WALK_UP = "up";
constexpr std::string_view EXIT = "exit";
void InputSystem::init(wv::AssetManager &, entt::registry &registry) {
  input = &registry.ctx().get<wv::InputContext>();
  input->bind_action(WALK_LEFT, SDL_SCANCODE_A);
  input->bind_action(WALK_DOWN, SDL_SCANCODE_S);
  input->bind_action(WALK_RIGHT, SDL_SCANCODE_D);
  input->bind_action(WALK_UP, SDL_SCANCODE_W);
  input->bind_action(EXIT, SDL_SCANCODE_ESCAPE);
}

void InputSystem::update(void *, entt::registry &registry) {
  wv::Vector2 direction = {0, 0};
  if (input->is_triggered(WALK_LEFT)) {
    direction.x = -1;
  } else if (input->is_triggered(WALK_RIGHT)) {
    direction.x = 1;
  }
  if (input->is_triggered(WALK_UP)) {
    direction.y = -1;
  } else if (input->is_triggered(WALK_DOWN)) {
    direction.y = 1;
  }
  float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
  if (length > 0) {
    direction.x /= length;
    direction.y /= length;
  }

  auto &[deltaTime] = registry.ctx().get<wv::Time>();
  for (auto &&[entity, player, transform] : registry.view<const PlayerComponent, wv::TransformComponent>().each()) {
    transform.position.x += player.speed * direction.x * deltaTime;
    transform.position.y += player.speed * direction.y * deltaTime;
  }
}
void InputSystem::shutdown(entt::registry &) {
  input->unbind_action(WALK_LEFT);
  input->unbind_action(WALK_DOWN);
  input->unbind_action(WALK_RIGHT);
  input->unbind_action(WALK_UP);
  input->unbind_action(EXIT);
}