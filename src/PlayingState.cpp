#include "PlayingState.hpp"
#include "Game.hpp" 
#include "MenuState.hpp" 
#include "PauseState.hpp" 
#include <iostream> 
// constructor for game system and background music
PlayingState::PlayingState(MenuState& menu)
    : menuStateRef(menu)
{
    //give menu access to this game system instance
    menuStateRef.linkGameSystem(&gameSystem);

    gameSystem.init();

    // load background music 
    if (!backgroundMusic.openFromFile("assets/music.mp3"))
    {
        std::cerr << "cant load the music from assets/music.mp3.\n";
    }
    else
    {
        backgroundMusic.setLoop(true);

        backgroundMusic.setVolume(50.f);

        backgroundMusic.play();
    }
}

// game resources clean up
PlayingState::~PlayingState()
{
    gameSystem.clean();
}

void PlayingState::handleInput(Game& game, const sf::Event& event)
{
    // ESC pause and window closing handling 
    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        backgroundMusic.pause();

        unsigned int w = game.getWindow().getSize().x;

        unsigned int h = game.getWindow().getSize().y;

        game.pushState(std::make_unique<PauseState>(w, h, "assets/arial.ttf"));
    }
}
// main gameplay update loop
void PlayingState::update(Game& game, float dt)
{
    if (backgroundMusic.getStatus() == sf::SoundSource::Paused)
    {
        backgroundMusic.play(); 
    }
    // update all gameplay system
    gameSystem.update(dt, game.getWindow());
    // check for game over
    if (gameSystem.isGameOver())
    {      
        backgroundMusic.stop();

        // send score back to menu
        menuStateRef.setFinalScore(gameSystem.getScore());

      
        menuStateRef.setMenuState(GameState::GameOver);
    // removing playing state return us to Menu State
        game.popState();
    }
}
// render the game world via game system
void PlayingState::render(sf::RenderWindow& window)
{
    gameSystem.render(window);
}