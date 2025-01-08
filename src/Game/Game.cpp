//
// Created by nelson on 1/4/25.
//

#include "Game.hpp"
#include <optional>

#include "../States/MenuState.hpp"

Game::Game() : m_window(sf::VideoMode({1920, 1080}), "Game of Intrigue", sf::Style::Close)
{
    m_window.setFramerateLimit(144);
    pushState(std::make_unique<MenuState>(m_window));
}

void Game::run()
{
    while (m_window.isOpen())
    {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents()
{
    while (const std::optional event = m_window.pollEvent())
    {
        if (!event.has_value())
        {
            continue;
        }
        if (event->is<sf::Event::Closed>())
        {
            m_window.close();
            break;
        }
        if (!m_states.empty())
        {
            m_states.top()->handleEvent(event.value());
        }
    }
}

void Game::update()
{
    if (!m_states.empty())
    {
        m_states.top()->update();
    }
}

void Game::render()
{
    m_window.clear();
    if (!m_states.empty())
    {
        m_states.top()->render(m_window);
    }
    m_window.display();
}

void Game::pushState(std::unique_ptr<GameState> state)
{
    state->onEnter();
    m_states.push(std::move(state));
}

void Game::popState()
{
    if (!m_states.empty())
    {
        m_states.top()->onExit();
        m_states.pop();
    }
}

std::unique_ptr<GameState> &Game::getCurrentState()
{
    return m_states.top();
}
