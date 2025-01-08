#ifndef ASSETS_HPP
#define ASSETS_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>

class Assets {
public:
    static Assets &getInstance() {
        static Assets instance;
        return instance;
    }

    // No copy or move allowed
    Assets(const Assets &) = delete;

    Assets &operator=(const Assets &) = delete;

    // Load and manage textures
    const sf::Texture &getTexture(const std::string &name);

    void loadTexture(const std::string &name, const std::filesystem::path &filePath);


    // Load and manage sounds
    const sf::SoundBuffer &getSoundBuffer(const std::string &name);

    void loadSoundBuffer(const std::string &name, const std::filesystem::path &filePath);

    // Clean up unused resources
    void unloadAll();

    // Load and manage ImGui fonts
    void loadImGuiFont(const std::filesystem::path &filePath, float size);
    void clearImGuiFonts();

private:
    Assets() = default; // Private constructor
    ~Assets() = default;

    std::map<std::string, sf::Texture> m_textures;
    std::map<std::string, sf::SoundBuffer> m_soundBuffers;
};

#endif
