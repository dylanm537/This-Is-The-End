#include "PlayingState.hpp"
#include "Game.hpp" 
#include "MenuState.hpp" 

PlayingState::PlayingState(MenuState& menu)
    : menuStateRef(menu)
{
    // Link the newly created GameSystem instance to the MenuState for score access
    menuStateRef.linkGameSystem(&gameSystem);
    gameSystem.init();
}

void PlayingState::handleInput(Game& /*game*/, const sf::Event& /*event*/)\
{
    // Player input is handled within the GameSystem::update function
}

void PlayingState::update(Game& game, float dt)
{
    gameSystem.update(dt, game.getWindow());

    if (gameSystem.isGameOver())
    {
        // --- FIX: Save the final score value to MenuState before destruction ---
        menuStateRef.setFinalScore(gameSystem.getScore());

        // When game is over, we pop this state, returning to the MenuState below
        menuStateRef.menuState = GameState::GameOver;
        game.popState(); // This destroys gameSystem
    }
}

void PlayingState::render(sf::RenderWindow& window)
{
    // The Game class handles clearing and displaying. PlayingState only calls render.
    gameSystem.render(window);
}