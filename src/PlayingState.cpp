#include "PlayingState.hpp"
#include "Game.hpp" 
#include "MenuState.hpp" 
#include "PauseState.hpp" 
#include <iostream> 

PlayingState::PlayingState(MenuState& menu)
    : menuStateRef(menu)
{
    menuStateRef.linkGameSystem(&gameSystem);
    gameSystem.init();

    // --- Music Loading and Playback ---
    if (!backgroundMusic.openFromFile("assets/music.mp3"))
    {
        std::cerr << "ERROR: Failed to load background music from assets/music.mp3.\n";
    }
    else
    {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50.f);
        backgroundMusic.play();
    }
    // ---------------------------------------
}

// ? The Destructor Definition (which now matches the declaration in the .hpp)
PlayingState::~PlayingState()
{
    // This cleans up GameSystem resources, preventing the original crash (exit code 0)
    gameSystem.clean();
}

void PlayingState::handleInput(Game& game, const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        backgroundMusic.pause();

        unsigned int w = game.getWindow().getSize().x;
        unsigned int h = game.getWindow().getSize().y;

        game.pushState(std::make_unique<PauseState>(w, h, "assets/arial.ttf"));
    }
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

        // 1. Pass the score to the MenuState below
        menuStateRef.setFinalScore(gameSystem.getScore());

        // 2. Tell the MenuState it should now display the Game Over screen/save score
        menuStateRef.setMenuState(GameState::GameOver);

        // 3. Pop the PlayingState to return control to the MenuState
        game.popState();
    }
}

void PlayingState::render(sf::RenderWindow& window)
{
    gameSystem.render(window);
}