#pragma once
#include "State.hpp"
#include <SFML/Graphics.hpp>

class Game; // Forward declaration

class PauseState : public State
{
public:
    // Constructor takes window dimensions and font path for text setup
    PauseState(unsigned int windowW, unsigned int windowH, const std::string& fontPath);

    // Detects ESC key to unpause
    void handleInput(Game& game, const sf::Event& event) override;

    // Does nothing, which stops the game time (dt is ignored)
    void update(Game& game, float dt) override;

    // Draws the PAUSED overlay on the screen
    void render(sf::RenderWindow& window) override;

private:
    sf::Font font;
    sf::Text pauseText;
    sf::Text instructionsText;
};