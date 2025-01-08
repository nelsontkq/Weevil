#include "Assets.hpp"
#include <iostream>
#include <imgui.h>
#include <imgui-SFML.h>

// Load texture
void Assets::loadTexture(const std::string &name, const std::filesystem::path &filePath) {
    sf::Texture texture;
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error loading texture: " << filePath << std::endl;
        return;
    }
    m_textures[name] = std::move(texture);
}

const sf::Texture &Assets::getTexture(const std::string &name) {
    return m_textures.at(name);
}


// Load sound
void Assets::loadSoundBuffer(const std::string &name, const std::filesystem::path &filePath) {
    sf::SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile(filePath)) {
        std::cerr << "Error loading sound: " << filePath << std::endl;
        return;
    }
    m_soundBuffers[name] = soundBuffer;
}

const sf::SoundBuffer &Assets::getSoundBuffer(const std::string &name) {
    return m_soundBuffers.at(name);
}

// Unload all resources
void Assets::unloadAll() {
    m_textures.clear();
    m_soundBuffers.clear();
}

// Load ImGui font
void Assets::loadFont(const std::filesystem::path &filePath, float size) {
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF(filePath.string().c_str(), size);
    ImGui::SFML::UpdateFontTexture();
}

// Clear all ImGui fonts
void Assets::clearFonts() {
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->Clear();
    ImGui::SFML::UpdateFontTexture(); // Update font texture after clearing
}
