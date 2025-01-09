//
// Created by nelson on 1/4/25.
//

#include "Game.hpp"
#include <optional>
#include "../Resources/Logger.hpp"

#include "../States/MenuState.hpp"

Game::Game() : m_window(sf::VideoMode({1920, 1080}), "Game of Intrigue", sf::Style::Close)
{
}

void Game::run()
{
    if(!ImGui::SFML::Init(m_window)) {
        throw std::runtime_error("Failed to initialize ImGui-SFML");
    }
    Logger::getInstance().log("Game started");
    Assets::getInstance().loadDebugMode();
    m_window.setFramerateLimit(144);
    pushState(std::make_unique<MenuState>(m_window));
    while (m_window.isOpen())
    {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed)
        {
            m_window.close();
            break;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::F3)
            {
                m_debugMode = !m_debugMode;
                if (m_debugMode)
                    Logger::getInstance().log("Debug mode enabled");
                else
                    Logger::getInstance().log("Debug mode disabled");
            }
        }
        if (!m_states.empty())
        {
            m_states.top()->handleEvent(event);
        }
    }
} 

void Game::update()
{
    ImGui::SFML::Update(m_window, m_deltaClock.restart());
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
    if (m_debugMode)
    {
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.3f);
        ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                  ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove |
                                  ImGuiWindowFlags_NoSavedSettings);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(10, 50), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        ImGui::Begin("Game Log");

        const auto& messages = Logger::getInstance().getMessages();
        for (const auto& msg : messages)
        {
            ImGui::TextWrapped("%s", msg.c_str());
        }

        if (ImGui::Button("Clear"))
        {
            Logger::getInstance().clear();
        }

        ImGui::End();
    }

    ImGui::SFML::Render(m_window);
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
