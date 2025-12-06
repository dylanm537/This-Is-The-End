#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>

class Game;

class PauseState : public State // pause state
{
public:  //no update gameplay, playing state is frozen 
    
    PauseState(unsigned int windowW, unsigned int windowH, const std::string& fontPath);

    //ESC still quits game when paused 
    void handleInput(Game& game, const sf::Event& event) override;

    // no gameplay update occurs while paused
    void update(Game& game, float dt) override;
    // draws a dark overlay and pause text
    
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;

    sf::Text pauseText;

    sf::Text instructionsText;
};