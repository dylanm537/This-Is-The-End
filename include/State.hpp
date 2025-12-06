#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Game; 
// interface for all game state
class State
{
public:
    virtual ~State() = default;
    //handles keyboard & mouse/
    virtual void handleInput(Game& game, const sf::Event& event) = 0;
    // updates the game logic using Delta Time
    virtual void update(Game& game, float dt) = 0;
    // shows everything belonging to the currently state 
    virtual void render(sf::RenderWindow& window) = 0;
};