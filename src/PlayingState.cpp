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


PlayingState::~PlayingState()
{
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
    if (backgroundMusic.getStatus() == sf::SoundSource::Paused)
    {
        backgroundMusic.play(); 
    }

    gameSystem.update(dt, game.getWindow());

    if (gameSystem.isGameOver())
    {      
        backgroundMusic.stop();


        menuStateRef.setFinalScore(gameSystem.getScore());

      
        menuStateRef.setMenuState(GameState::GameOver);
    
        game.popState();
    }
}

void PlayingState::render(sf::RenderWindow& window)
{
    gameSystem.render(window);
}