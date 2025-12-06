#pragma once
#include <SFML/Graphics.hpp>
#include <stack>
#include <memory>
#include "State.hpp"
#include "game_parameters.hpp"

class Game
{  //this class has game the gae window state ( stack of states)
public:
    Game();
    void run();
    // the game loop
  
    void pushState(std::unique_ptr<State> state);

    void popState();
    void changeState(std::unique_ptr<State> state);
    // access to render window crosshair 
    sf::RenderWindow& getWindow() { return window; }

    const sf::Sprite& getCrosshair() const { return crosshair; }

private:
    sf::RenderWindow window;

    std::stack<std::unique_ptr<State>> states;

   // crosshair graphics
    sf::Texture crosshairTex;
    sf::Sprite crosshair;
};