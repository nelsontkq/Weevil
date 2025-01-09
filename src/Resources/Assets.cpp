#include "Assets.hpp"
#include "Logger.hpp"
#include <iostream>
#include <filesystem>

Assets &Assets::getInstance()
{
    static Assets instance;
    return instance;
}

Assets::Assets()
{
}

std::shared_ptr<sf::Texture> Assets::loadTexture(const std::string &filePath)
{
    auto it = m_textures.find(filePath);
    if (it != m_textures.end())
    {
        if (auto sharedTex = it->second.lock())
        {
            return sharedTex;
        }
    }

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(filePath))
    {
        Logger::getInstance().log("Error loading texture: " + filePath);
        return nullptr;
    }

    m_textures[filePath] = texture;
    return texture;
}

std::shared_ptr<sf::SoundBuffer> Assets::loadSoundBuffer(const std::string &filePath)
{
    auto it = m_soundBuffers.find(filePath);
    if (it != m_soundBuffers.end())
    {
        if (auto sharedBuffer = it->second.lock())
        {
            return sharedBuffer;
        }
    }

    auto soundBuffer = std::make_shared<sf::SoundBuffer>();
    if (!soundBuffer->loadFromFile(filePath))
    {
        Logger::getInstance().log("Error loading sound buffer: " + filePath);
        return nullptr;
    }

    m_soundBuffers[filePath] = soundBuffer;
    return soundBuffer;
}

void Assets::loadDebugMode()
{
    // fonts
    auto &io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("assets/fonts/OpenSans-Variable.ttf", 24.0f);
    ImGui::SFML::UpdateFontTexture();
}

ImFont* Assets::getFont(FontType fontType)
{
    switch (fontType)
    {
    case FontType::Menu:
        return ImGui::GetIO().Fonts->Fonts[0];
    default:
        return nullptr;
    }
}
