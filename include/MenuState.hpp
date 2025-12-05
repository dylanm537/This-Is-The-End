#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "State.hpp"
#include "game_system.hpp"


enum class GameState {
    Start,
    NameEntry,
    Playing,
    GameOver,
    Quit
};

class MenuState : public State
{
public:
    MenuState(unsigned int windowW, unsigned int windowH, const std::string& fontPath);

    void handleInput(Game& game, const sf::Event& event) override;
    void update(Game& game, float dt) override;
    void render(sf::RenderWindow& window) override;

    // Internal state tracking
    GameState menuState;
    std::string playerName;

 
    void linkGameSystem(GameSystem* gs) { gameSystem = gs; }

private:
    void drawStart(sf::RenderWindow& w);
    void drawNameEntry(sf::RenderWindow& w);
    void drawGameOver(sf::RenderWindow& w, int score);

    sf::Font font;
    unsigned int w, h;
    sf::Text titleText;
    sf::Text namePrompt;
    sf::Text nameText;

    GameSystem* gameSystem = nullptr;
};