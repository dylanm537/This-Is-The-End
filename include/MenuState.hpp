#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector> 
#include "State.hpp"
#include "game_system.hpp"

// Structure to hold one score entry (name and score)
struct ScoreEntry {
    std::string name{}; // Fix C26495: Always initialize member variables
    int score = 0;      // Fix C26495: Always initialize member variables
};

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

    // Allows the PlayingState to link its GameSystem instance to get the final score.
    void linkGameSystem(GameSystem* gs) { gameSystem = gs; }

    // Method to store the score from the finished game
    void setFinalScore(int score) { finalScore = score; }

    // FIX: Public setter to allow PlayingState to change the menu's internal state
    void setMenuState(GameState newState) { menuState = newState; }

private:
    // New methods for handling the list of high scores
    void loadScores();
    void saveScore(int currentScore);

    void drawStart(sf::RenderWindow& w);
    void drawNameEntry(sf::RenderWindow& w);
    void drawGameOver(sf::RenderWindow& w, int score);

    // Internal state tracking
    GameState menuState = GameState::Start;
    std::string playerName; // Name being typed or the name that just scored

    sf::Font font;
    unsigned int w, h;
    sf::Text titleText;
    sf::Text namePrompt;
    sf::Text inputField;

    // High Score Data
    std::vector<ScoreEntry> topScores;
    int finalScore = 0;
    GameSystem* gameSystem = nullptr;
    const unsigned int maxNameLength = 12;

    // Constants for high score management
    static constexpr int MAX_SCORES_TO_KEEP = 5;
    const std::string SCORES_FILE = "scores.txt";
};