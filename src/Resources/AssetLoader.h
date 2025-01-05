//
// Created by nelson on 1/4/25.
//

#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetLoader {
public:
    static AssetLoader& getInstance();
    const sf::Texture& getTexture(const std::string& name);
    void loadTexture(const std::string& name, const std::string& filename);

private:
    std::map<std::string, sf::Texture> m_textures;
};

#endif //ASSETLOADER_H
