#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector> 
#include "State.hpp"
#include "game_system.hpp"

// score entry : stores players name and their score
//saving the top high scores

struct ScoreEntry {
    std::string name{}; 
    int score = 0;      
};

//game state enumerator identifies the current menu mode
enum class GameState {
    Start,
    NameEntry,
    Playing,
    GameOver,
    Quit
};
// renders screen and takes name 
// displaying and saving high scores
//also transitions to playing state 
class MenuState : public State
{
public:
    MenuState(unsigned int windowW, unsigned int windowH, const std::string& fontPath);
    // state interface implementation 
    void handleInput(Game& game, const sf::Event& event) override;
    
    void update(Game& game, float dt) override;

    void render(sf::RenderWindow& window) override;

   // provides playing state reference Game system
    void linkGameSystem(GameSystem* gs) { gameSystem = gs; }
    // sets score at end 
    void setFinalScore(int score) { finalScore = score; }

    // game over prompt state 
    void setMenuState(GameState newState) { menuState = newState; }

private:
    // load and save high score data 
    void loadScores();
    void saveScore(int currentScore);
    // shows the helpers on the menu screen
    void drawStart(sf::RenderWindow& w);
    void drawNameEntry(sf::RenderWindow& w);
    void drawGameOver(sf::RenderWindow& w, int score);

   // menu state machine
    GameState menuState = GameState::Start;
    std::string playerName; // string for player name 

    sf::Font font;
    // UI properties
    unsigned int w, h;

    sf::Text titleText;
    sf::Text namePrompt;
    sf::Text inputField;

    
    std::vector<ScoreEntry> topScores; // high score storage 
    int finalScore = 0;
    GameSystem* gameSystem = nullptr;

    const unsigned int maxNameLength = 12; // maximum 12 char for name 

    static constexpr int MAX_SCORES_TO_KEEP = 5;
    const std::string SCORES_FILE = "scores.txt";
};