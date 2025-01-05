//
// Created by nelson on 1/4/25.
//
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Entity {
public:
    virtual ~Entity() = default;
    virtual void update() = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};

#endif