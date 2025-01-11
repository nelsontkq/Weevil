#pragma once

#include <memory>

// Forward declarations of game states and systems
class GameState;
class MainMenuState;
class GameplayState;
class EventState;
class PauseState;
class ECSManager;
class LLMController;

class Game {
public:
    // Constructor and Destructor
    Game();
    ~Game();

    // Main game loop
    void run();

private:
    // Method to initialize the game
    void initialize();

    // Method to handle events
    void handleEvents();

    // Method to update the game state
    void update();

    // Method to render the game
    void render();

    // Method to clean up resources
    void cleanup();

    // Member variables
    // Current game state pointer
    std::unique_ptr<GameState> currentState;

    // Game states
    std::unique_ptr<MainMenuState> mainMenuState;
    std::unique_ptr<GameplayState> gameplayState;
    std::unique_ptr<EventState> eventState;
    std::unique_ptr<PauseState> pauseState;

    // Systems and controllers
    std::unique_ptr<ECSManager> ecsManager;
    std::unique_ptr<LLMController> llmController;
    // Other systems like AI, Rendering, Input, etc.

    // Flag to control the main loop
    bool isRunning;
};
