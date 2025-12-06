#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "State.hpp"
#include "game_system.hpp"

// Keep GameState enum to manage internal menu flow
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

    // Allows the PlayingState to link its GameSystem instance to get the final score.
    void linkGameSystem(GameSystem* gs) { gameSystem = gs; }

    // --- NEW: Method to store the score from the finished game ---
    void setFinalScore(int score) { finalScore = score; }

private:
    void loadHighscore();
    void saveHighscore(int currentScore);

    void drawStart(sf::RenderWindow& w);
    void drawNameEntry(sf::RenderWindow& w);
    void drawGameOver(sf::RenderWindow& w, int score);

    sf::Font font;
    unsigned int w, h;
    sf::Text titleText;
    sf::Text namePrompt;
    sf::Text nameText;

    GameSystem* gameSystem = nullptr;

    // --- NEW: Stored score from the last game ---
    int finalScore = 0;

    // High Score Data
    std::string highscoreName;
    int highscore = 0;

    // Blink Timer
    float cursorBlinkTimer = 0.f;
};