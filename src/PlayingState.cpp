#include "PlayingState.hpp"
#include "Game.hpp" 
#include "MenuState.hpp" 

PlayingState::PlayingState(MenuState& menu)
    : menuStateRef(menu)
{

    menuStateRef.linkGameSystem(&gameSystem);
    gameSystem.init();
}

void PlayingState::handleInput(Game& /*game*/, const sf::Event& /*event*/)
{

}

void PlayingState::update(Game& game, float dt)
{
    gameSystem.update(dt, game.getWindow());

    if (gameSystem.isGameOver())
    {

        menuStateRef.menuState = GameState::GameOver;
        game.popState();
    }
}

void PlayingState::render(sf::RenderWindow& window)
{

    gameSystem.render(window);
}