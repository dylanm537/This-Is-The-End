#include "PlayingState.hpp"
#include "Game.hpp" 
#include "MenuState.hpp" 
#include "PauseState.hpp" 
#include <iostream> // <-- FIX: Required for std::cerr
// sf::Music include is usually handled by PlayingState.hpp

PlayingState::PlayingState(MenuState& menu)
    : menuStateRef(menu)
{
    // Link the newly created GameSystem instance to the MenuState for score access
    menuStateRef.linkGameSystem(&gameSystem);
    gameSystem.init();

    // --- Music Loading and Playback ---
    // Using the .mp3 file and path you specified
    if (!backgroundMusic.openFromFile("assets/music.mp3"))
    {
        // Handle error: File not found or failed to load
        std::cerr << "ERROR: Failed to load background music from assets/music.mp3.\n";
    }
    else
    {
        // Configure playback
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50.f);

        // Start playing
        backgroundMusic.play();
    }
    // ---------------------------------------
}

void PlayingState::handleInput(Game& game, const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        // When pausing, we PAUSE the music playback
        backgroundMusic.pause();

        unsigned int w = game.getWindow().getSize().x;
        unsigned int h = game.getWindow().getSize().y;

        game.pushState(std::make_unique<PauseState>(w, h, "assets/arial.ttf"));
    }

    // Player input is handled within the GameSystem::update function
}

void PlayingState::update(Game& game, float dt)
{
    // Resume music if it was paused (i.e., we just returned from the PauseState)
    if (backgroundMusic.getStatus() == sf::SoundSource::Paused)
    {
        backgroundMusic.play(); // Resume playback
    }

    gameSystem.update(dt, game.getWindow());

    if (gameSystem.isGameOver())
    {
        // Stop the music when the game ends
        backgroundMusic.stop();

        menuStateRef.setFinalScore(gameSystem.getScore());
        menuStateRef.menuState = GameState::GameOver;
        game.popState();
    }
}

void PlayingState::render(sf::RenderWindow& window)
{
    gameSystem.render(window);
}