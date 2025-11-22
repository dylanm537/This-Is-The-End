// Menu.h
#pragma once
#include <SFML/Graphics.hpp>

enum class GameState {
    Start,
    Playing,
    GameOver,
    Quit
};

class Menu {
public:
    Menu(unsigned int windowW, unsigned int windowH, sf::Texture& bgTexture, const std::string& fontPath);

    void drawStart(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int score);

    GameState state;

private:
    sf::Font font;
    sf::Sprite bgSprite;
    unsigned int w, h;
};
