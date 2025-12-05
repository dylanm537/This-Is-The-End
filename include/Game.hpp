#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include <memory>
#include "State.hpp"
#include "game_parameters.hpp"

class Game
{
public:
    Game();
    void run();

    // State management functions
    void pushState(std::unique_ptr<State> state);
    void popState();
    void changeState(std::unique_ptr<State> state);

    sf::RenderWindow& getWindow() { return window; }
    const sf::Sprite& getCrosshair() const { return crosshair; }

private:
    sf::RenderWindow window;
    std::stack<std::unique_ptr<State>> states;

    // Crosshair
    sf::Texture crosshairTex;
    sf::Sprite crosshair;
};