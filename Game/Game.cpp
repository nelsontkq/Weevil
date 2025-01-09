#include "Game.hpp"
#include "States/MainMenuState.hpp"
#include "States/GameplayState.hpp"
// Include other states and systems as needed

Game::Game()
    : isRunning(true)
{
    // Initialize systems and controllers
    ecsManager = std::make_unique<ECSManager>();
    llmController = std::make_unique<LLMController>();
    // Initialize other systems

    // Create game states
    mainMenuState = std::make_unique<MainMenuState>(this);
    gameplayState = std::make_unique<GameplayState>(this);
    // Initialize other states

    // Set the initial state
    currentState = std::move(mainMenuState);
}

Game::~Game()
{
    cleanup();
}

void Game::run()
{
    initialize();

    while (isRunning) {
        handleEvents();
        update();
        render();
    }

    cleanup();
}

void Game::initialize()
{
    // Initialize asset manager and load assets
    // Initialize ImGui or other UI systems
    // Initialize LLMController connection if needed
}

void Game::handleEvents()
{
    // Poll and process events
    // Pass events to the current game state
    currentState->handleEvents();

    // Check for global exit conditions
    // if (exit condition) isRunning = false;
}

void Game::update()
{
    // Update current game state
    currentState->update();

    // Update ECS systems as needed
    ecsManager->updateSystems();

    // Handle state transitions if required
    // Example:
    // if (currentState->isStateFinished()) {
    //     // Transition to the next state
    // }
}

void Game::render()
{
    // Clear the screen

    // Render current game state
    currentState->render();

    // Render UI via ImGui
    // ImGui rendering calls

    // Swap buffers / Present the frame
}

void Game::cleanup()
{
    // Clean up resources
    // Destroy ImGui context if necessary
    // Close LLMController connection if necessary
}
