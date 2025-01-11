#include "RenderSystem.hpp"
#include "../Components/PositionComponent.hpp"
#include "../Components/RenderComponent.hpp"

void RenderSystem::render(entt::registry& registry, sf::RenderWindow& window) {
    auto view = registry.view<PositionComponent, RenderComponent>();

    for (auto entity : view) {
        auto& position = view.get<PositionComponent>(entity);
        auto& render = view.get<RenderComponent>(entity);

        render.sprite.setPosition(position.x, position.y);
        window.draw(render.sprite);
    }
}
