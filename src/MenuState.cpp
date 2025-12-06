#include "MenuState.hpp"
#include "Game.hpp"
#include "PlayingState.hpp"
#include <iostream>
#include <fstream> 

using namespace std;

// =========================================================================
// HIGH SCORE IMPLEMENTATION
// =========================================================================

void MenuState::loadHighscore()
{
    // ... (No change)
    ifstream file("highscore.txt");
    if (file.is_open())
    {
        if (!(file >> highscoreName >> highscore))
        {
            highscoreName = "N/A";
            highscore = 0;
        }
        file.close();
    }
    else
    {
        highscoreName = "N/A";
        highscore = 0;
    }
}

void MenuState::saveHighscore(int currentScore)
{
    // ... (No change)
    if (currentScore > highscore)
    {
        highscore = currentScore;
        highscoreName = playerName;

        ofstream file("highscore.txt");
        if (file.is_open())
        {
            file << highscoreName << " " << highscore;
            file.close();
        }
    }
}

// =========================================================================
// CONSTRUCTOR (No change)
// =========================================================================
MenuState::MenuState(unsigned int windowW, unsigned int windowH, const std::string& fontPath)
    : menuState(GameState::Start)
    , w(windowW)
    , h(windowH)
    , playerName("")
    , finalScore(0) // Initialize new member
    , cursorBlinkTimer(0.f)
{
    if (!font.loadFromFile(fontPath))
    {
        cerr << "Warning: Could not load font\n";
    }

    loadHighscore();

    // ... (Text setup)
    titleText.setFont(font);
    titleText.setCharacterSize(52);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setString("THIS IS THE END?");
    titleText.setPosition(w / 2.f - titleText.getGlobalBounds().width / 2.f, 120.f);

    namePrompt.setFont(font);
    namePrompt.setCharacterSize(32);
    namePrompt.setFillColor(sf::Color::White);
    namePrompt.setString("ENTER YOUR NAME:");
    namePrompt.setPosition(w / 2.f - 180.f, 260.f);

    nameText.setFont(font);
    nameText.setCharacterSize(28);
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setPosition(w / 2.f - 150.f, 310.f);
}

// =========================================================================
// INPUT/UPDATE/RENDER
// =========================================================================

void MenuState::handleInput(Game& game, const sf::Event& event)
{
    // ... (No change)
    if (menuState == GameState::Start)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            menuState = GameState::NameEntry;
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            game.getWindow().close();
        }
    }
    else if (menuState == GameState::NameEntry)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode < 128)
            {
                if (event.text.unicode == '\b') // Backspace
                {
                    if (!playerName.empty())
                        playerName.pop_back();
                }
                else if (playerName.size() < 10 && event.text.unicode != '\r' && event.text.unicode != '\n') // Limit name length and ignore carriage return/newline
                {
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            if (playerName.empty())
                playerName = "NO_NAME";

            menuState = GameState::Playing;
            game.pushState(make_unique<PlayingState>(*this));
        }
    }
    else if (menuState == GameState::GameOver)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            menuState = GameState::NameEntry;
            playerName.clear();
            finalScore = 0; // Reset score for next game
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            game.getWindow().close();
        }
    }
}

void MenuState::update(Game& game, float dt)
{
    // This is called *after* PlayingState has popped itself and returned control.
    if (menuState == GameState::GameOver)
    {
        // 1. Check and save high score using the safely stored finalScore.
        saveHighscore(finalScore);

        // 2. Clear the pointer to the old GameSystem instance for the next game (Optional, but safe)
        if (gameSystem)
        {
            gameSystem = nullptr;
        }
    }

    // Cursor Blinking Logic (No functional change)
    if (menuState == GameState::NameEntry)
    {
        cursorBlinkTimer += dt;
        const float BLINK_INTERVAL = 0.5f;

        if (cursorBlinkTimer >= BLINK_INTERVAL)
        {
            cursorBlinkTimer -= BLINK_INTERVAL;
        }

        bool showCursor = cursorBlinkTimer < (BLINK_INTERVAL / 2.f);
        nameText.setString(playerName + (showCursor ? "_" : ""));
    }
}

void MenuState::render(sf::RenderWindow& window)
{
    if (menuState == GameState::Start)
        drawStart(window);
    else if (menuState == GameState::NameEntry)
        drawNameEntry(window);
    else if (menuState == GameState::GameOver)
        // --- FIX: Use the stored finalScore member variable ---
        drawGameOver(window, finalScore);
}


// =========================================================================
// DRAWING FUNCTIONS (No functional change here, passing the correct score now)
// =========================================================================

void MenuState::drawStart(sf::RenderWindow& w)
{
    w.clear(sf::Color(15, 15, 30));
    w.draw(titleText);

    // Display High Score on Start screen
    sf::Text hs("PERSONAL BEST", font, 24);
    // ... (rest of drawStart)
    hs.setFillColor(sf::Color(255, 200, 50));
    hs.setPosition(w.getSize().x / 2.f - hs.getGlobalBounds().width / 2.f, 250.f);
    w.draw(hs);

    sf::Text hsText(highscoreName + " - " + std::to_string(highscore), font, 28);
    hsText.setFillColor(sf::Color::Yellow);
    hsText.setPosition(w.getSize().x / 2.f - hsText.getGlobalBounds().width / 2.f, 285.f);
    w.draw(hsText);

    sf::Text info("Press ENTER to Continue\nPress ESC to Quit", font, 22);
    info.setFillColor(sf::Color(200, 200, 220));
    info.setPosition(w.getSize().x / 2.f - info.getGlobalBounds().width / 2.f, 380.f);

    w.draw(info);
}

void MenuState::drawNameEntry(sf::RenderWindow& w)
{
    w.clear(sf::Color(15, 15, 30));

    w.draw(namePrompt);

    // Use nameText's string that was set in update() for cursor blinking
    w.draw(nameText);

    sf::Text info("Press ENTER to Start Game", font, 20);
    info.setFillColor(sf::Color(200, 200, 200));
    info.setPosition(w.getSize().x / 2.f - info.getGlobalBounds().width / 2.f, 380.f);
    w.draw(info);
}


void MenuState::drawGameOver(sf::RenderWindow& w, int score)
{
    w.clear(sf::Color(30, 0, 0));

    sf::Text gameOver("GAME OVER", font, 60);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setStyle(sf::Text::Bold);
    gameOver.setPosition(w.getSize().x / 2.f - gameOver.getGlobalBounds().width / 2.f, 100.f);
    w.draw(gameOver);

    // 1. Current Score
    sf::Text finalScoreText("YOUR SCORE: " + std::to_string(score) + " (" + playerName + ")", font, 36);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setPosition(w.getSize().x / 2.f - finalScoreText.getGlobalBounds().width / 2.f, 200.f);
    w.draw(finalScoreText);

    // 2. Personal Best Score
    sf::Text personalBest("PERSONAL BEST", font, 28);
    personalBest.setFillColor(sf::Color(255, 200, 50));
    personalBest.setPosition(w.getSize().x / 2.f - personalBest.getGlobalBounds().width / 2.f, 300.f);
    w.draw(personalBest);

    sf::Text bestScoreText(highscoreName + " - " + std::to_string(highscore), font, 32);

    // Highlight if the player just got the new high score
    if (score == highscore && highscoreName == playerName)
    {
        bestScoreText.setString("NEW BEST: " + highscoreName + " - " + std::to_string(highscore));
        bestScoreText.setFillColor(sf::Color::Green);
    }
    else
    {
        bestScoreText.setString(highscoreName + " - " + std::to_string(highscore));
        bestScoreText.setFillColor(sf::Color::Yellow);
    }

    bestScoreText.setPosition(w.getSize().x / 2.f - bestScoreText.getGlobalBounds().width / 2.f, 340.f);
    w.draw(bestScoreText);

    // 3. Instructions
    sf::Text info("Press ENTER to Play Again\nPress ESC to Quit", font, 22);
    info.setFillColor(sf::Color(200, 200, 220));
    info.setPosition(w.getSize().x / 2.f - info.getGlobalBounds().width / 2.f, 450.f);
    w.draw(info);
}