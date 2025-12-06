#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector> 
#include "State.hpp"
#include "game_system.hpp"


struct ScoreEntry {
    std::string name{}; 
    int score = 0;      
};


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

   
    void linkGameSystem(GameSystem* gs) { gameSystem = gs; }

    void setFinalScore(int score) { finalScore = score; }

    
    void setMenuState(GameState newState) { menuState = newState; }

private:
    
    void loadScores();
    void saveScore(int currentScore);

    void drawStart(sf::RenderWindow& w);
    void drawNameEntry(sf::RenderWindow& w);
    void drawGameOver(sf::RenderWindow& w, int score);

   
    GameState menuState = GameState::Start;
    std::string playerName; 

    sf::Font font;

    unsigned int w, h;

    sf::Text titleText;
    sf::Text namePrompt;
    sf::Text inputField;

    
    std::vector<ScoreEntry> topScores;
    int finalScore = 0;
    GameSystem* gameSystem = nullptr;

    const unsigned int maxNameLength = 12;

    static constexpr int MAX_SCORES_TO_KEEP = 5;
    const std::string SCORES_FILE = "scores.txt";
};