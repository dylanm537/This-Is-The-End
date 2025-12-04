#pragma once
#include <SFML/Graphics.hpp>

class Menu;

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
    void linkMenu(Menu* m);

    bool isGameOver() const;

public: // You chose option 2
    static sf::Texture spritesheet;
};
