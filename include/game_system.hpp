#pragma once
#include <SFML/Graphics.hpp>

class GameSystem
{
public:
    GameSystem() = default;

    void init(); // setup textures,map, player, zombie, walls and variable
    void reset(); // restart game to initial codition
    void clean(); // free allocated resources 

    void update(const float& dt, const sf::RenderWindow& window);

    void render(sf::RenderWindow& window);
    // score and game over playing state / menu state
    int getScore() const;

    bool isGameOver() const;
    // shared spritesheet 
public:
    static sf::Texture spritesheet;
};