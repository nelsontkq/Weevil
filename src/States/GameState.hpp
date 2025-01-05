//
// Created by nelson on 1/4/25.
//

#ifndef GAMESTATE_HP
#define GAMESTATE_HP

class GameState {
public:
    virtual ~GameState() = default;

    virtual void handleEvent(const std::optional<sf::Event> &event) = 0;

    virtual void update() = 0;

    virtual void render(sf::RenderWindow &window) = 0;
};


#endif //GAMESTATE_HP
