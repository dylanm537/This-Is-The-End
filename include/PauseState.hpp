#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>

class Game;

class PauseState : public State
{
public:
    
    PauseState(unsigned int windowW, unsigned int windowH, const std::string& fontPath);

    
    void handleInput(Game& game, const sf::Event& event) override;

    
    void update(Game& game, float dt) override;

    
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text pauseText;
    sf::Text instructionsText;
};