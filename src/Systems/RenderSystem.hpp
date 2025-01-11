#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class RenderSystem {
public:
    void render(entt::registry& registry, sf::RenderWindow& window);
};

#endif // RENDER_SYSTEM_HPP
