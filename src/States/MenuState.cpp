#include "../Resources/Assets.hpp"
#include "MenuState.hpp"
#include <imgui.h>
#include "../Resources/Logger.hpp"

MenuState::MenuState(sf::RenderWindow &window, entt::registry &registry) : m_window(window), m_registry(registry)
{
}

void MenuState::onEnter()
{
    // Load state-specific assets
    auto &assets = Assets::getInstance();
    assets.loadDebugMode();
    Logger::getInstance().log("Entered MenuState");
    auto texture = assets.loadTexture("assets/textures/menu_background.png");
    if (!texture) {
        Logger::getInstance().log("Failed to load background texture");
        return;
    }

    auto backgroundEntity = m_registry.create();
    auto& renderComp = m_registry.emplace<RenderComponent>(backgroundEntity);
    renderComp.sprite.setTexture(*texture);
    renderComp.sprite.setScale(
        static_cast<float>(m_window.getSize().x) / renderComp.sprite.getLocalBounds().width,
        static_cast<float>(m_window.getSize().y) / renderComp.sprite.getLocalBounds().height
    );

    m_registry.emplace<PositionComponent>(backgroundEntity, 0.0f, 0.0f);
}

void MenuState::onExit()
{
}

void MenuState::handleEvent(const sf::Event &event)
{
}

void MenuState::update()
{
}

void MenuState::render(sf::RenderWindow &window)
{
    // Rendering will be handled by systems
}
