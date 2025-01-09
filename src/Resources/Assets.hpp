#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <imgui.h>
#include <imgui-SFML.h>

class Assets {
public:
    static Assets& getInstance();

    std::shared_ptr<sf::Texture> loadTexture(const std::string& filePath);
    std::shared_ptr<sf::SoundBuffer> loadSoundBuffer(const std::string& filePath);
    // ORDER MATTERS
    enum FontType {
        Menu = 0,
    };

    void loadDebugMode();
    ImFont* getFont(FontType fontType);

private:
    Assets();
    Assets(const Assets&) = delete;
    Assets& operator=(const Assets&) = delete;

    std::unordered_map<std::string, std::weak_ptr<sf::Texture>> m_textures;
    std::unordered_map<std::string, std::weak_ptr<sf::SoundBuffer>> m_soundBuffers;
};

#endif