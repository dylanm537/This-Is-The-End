#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Game; 

class State
{
public:
    virtual ~State() = default;

    virtual void handleInput(Game& game, const sf::Event& event) = 0;
    virtual void update(Game& game, float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};