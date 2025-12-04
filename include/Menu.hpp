#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class GameState {
    Start,
    NameEntry,
    Playing,
    GameOver,
    Quit
};

class Menu
{
public:
    Menu(unsigned int windowW, unsigned int windowH, const std::string& fontPath);

    void drawStart(sf::RenderWindow& w);
    void drawNameEntry(sf::RenderWindow& w);
    void drawGameOver(sf::RenderWindow& w, int score);

    GameState state;
    std::string playerName;

private:
    sf::Font font;

    unsigned int w, h;

    sf::Text titleText;
    sf::Text namePrompt;
    sf::Text nameText;
};
