#pragma once
#include <SFML/Graphics.hpp>

class GameSystem
{
public:
    GameSystem() = default;

    void init();
    void reset();
    void clean();

    void update(const float& dt, const sf::RenderWindow& window);

    void render(sf::RenderWindow& window);

    int getScore() const;

    bool isGameOver() const;

public:
    static sf::Texture spritesheet;
};