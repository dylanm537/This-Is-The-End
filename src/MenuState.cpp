#include "MenuState.hpp"
#include "Game.hpp"
#include "PlayingState.hpp"
#include <iostream>
#include <fstream> 
#include <algorithm> 

using namespace std;

namespace
{

    const std::string SCORES_FILE = "highscores.dat";
    const size_t MAX_HIGH_SCORES = 5;
    const size_t maxNameLength = 10;

    bool compareScores(const ScoreEntry& a, const ScoreEntry& b)
    {
        return a.score > b.score;
    }
}

void MenuState::loadScores()
{
    topScores.clear();

    ifstream file(SCORES_FILE);

    if (file.is_open())
    {
        ScoreEntry entry;

        while (file >> entry.name >> entry.score)
        {
            topScores.push_back(entry);
        }

        file.close();

        std::sort(topScores.begin(), topScores.end(), compareScores);
    }
}

void MenuState::saveScore(int currentScore)
{

    if (currentScore > 0 && !playerName.empty())
    {
 
        ScoreEntry newEntry;

        newEntry.name = playerName;
        newEntry.score = currentScore;

        topScores.push_back(newEntry);

        std::sort(topScores.begin(), topScores.end(), compareScores);

        if (topScores.size() > MAX_HIGH_SCORES)
        {
            topScores.resize(MAX_HIGH_SCORES);
        }


        ofstream file(SCORES_FILE);

        if (file.is_open())
        {
            for (const auto& entry : topScores)
            {
                file << entry.name << " " << entry.score << "\n";
            }
            file.close();
        }
    }
}


MenuState::MenuState(unsigned int windowW, unsigned int windowH, const std::string& fontPath)
    : w(windowW), h(windowH), menuState(GameState::Start)
{
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "cant load font " << fontPath << "\n";
    }

  
    titleText.setFont(font);
    titleText.setString("THIS IS THE END");
    titleText.setCharacterSize(100);
    titleText.setFillColor(sf::Color::Red);

    sf::FloatRect bounds = titleText.getLocalBounds();

    titleText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    titleText.setPosition((float)w / 2.f, 150.f);


    namePrompt.setFont(font);
    namePrompt.setString("Enter your name:");
    namePrompt.setCharacterSize(40);
    namePrompt.setFillColor(sf::Color::White);

    bounds = namePrompt.getLocalBounds();

    namePrompt.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    namePrompt.setPosition((float)w / 2.f, (float)h / 2.f - 50.f);

    inputField.setFont(font);
    inputField.setCharacterSize(50);
    inputField.setFillColor(sf::Color::Yellow);

    loadScores();
}

void MenuState::handleInput(Game& game, const sf::Event& event)
{
    if (menuState == GameState::Quit)
    {
        game.getWindow().close();
        return;
    }

    if (event.type == sf::Event::Closed)
    {
        menuState = GameState::Quit;
    }
    else if (menuState == GameState::Start || menuState == GameState::GameOver)
    {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            menuState = GameState::NameEntry;

            playerName.clear(); 
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            menuState = GameState::Quit;
        }
    }
    else if (menuState == GameState::NameEntry)
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode >= 32 && event.text.unicode <= 126)
            {
                if (playerName.length() < maxNameLength)
                {
                    playerName += static_cast<char>(event.text.unicode);
                }
            }
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace)
        {
            if (!playerName.empty())
            {
                playerName.pop_back();
            }
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            if (playerName.empty()) {
                playerName = "SURVIVOR";
            }

            menuState = GameState::Playing;

            game.pushState(std::make_unique<PlayingState>(*this));
        }


    }
}

void MenuState::update(Game& game, float dt)
{
    if (menuState == GameState::Quit)
    {
        game.getWindow().close();
    }
    else if (menuState == GameState::GameOver)
    {
        saveScore(finalScore);

        loadScores(); 

        menuState = GameState::Start;
    }
    else if (menuState == GameState::NameEntry)
    {

        static float timeElapsed = 0.f;

        timeElapsed += dt;

        bool cursorVisible = (static_cast<int>(timeElapsed * 2)) % 2 == 0;

        std::string textWithCursor = playerName;
        if (cursorVisible) {
            textWithCursor += "_";
        }

        inputField.setString(textWithCursor);

 
        sf::FloatRect bounds = inputField.getLocalBounds();

        inputField.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

        inputField.setPosition((float)w / 2.f, (float)h / 2.f + 10.f);
    }
}


void MenuState::render(sf::RenderWindow& window)
{
    if (menuState == GameState::Start)
    {
        drawStart(window);
    }
    else if (menuState == GameState::NameEntry)
    {
        drawNameEntry(window);
    }
}


void MenuState::drawStart(sf::RenderWindow& w)
{
    w.draw(titleText);

    sf::Text instructions("Press ENTER to start / ESC to quit", font, 40);

    instructions.setFillColor(sf::Color::White);

    sf::FloatRect bounds = instructions.getLocalBounds();

    instructions.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    instructions.setPosition((float)this->w / 2.f, (float)this->h / 2.f + 100.f);

    w.draw(instructions);

    if (finalScore > 0)
    {
        sf::Text lastScoreText("Last Score: " + std::to_string(finalScore) + " (" + playerName + ")", font, 36);

        lastScoreText.setFillColor(sf::Color::Cyan);

        bounds = lastScoreText.getLocalBounds();

        lastScoreText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        lastScoreText.setPosition((float)this->w / 2.f, (float)this->h / 2.f + 180.f);

        w.draw(lastScoreText);
    }

    sf::Text highscoreTitle("HIGH SCORES (TOP " + std::to_string(MAX_HIGH_SCORES) + ")", font, 30);

    highscoreTitle.setFillColor(sf::Color(255, 200, 50));

    bounds = highscoreTitle.getLocalBounds();
    highscoreTitle.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);

    highscoreTitle.setPosition((float)this->w / 2.f, 300.f);

    w.draw(highscoreTitle);

    float currentY = 350.f;
    for (size_t i = 0; i < topScores.size(); ++i)
    {
        const auto& entry = topScores[i];

        std::string rankStr = std::to_string(i + 1) + ". ";
        std::string scoreStr = entry.name + " - " + std::to_string(entry.score);

        sf::Text scoreText(rankStr + scoreStr, font, 28);
        scoreText.setFillColor(sf::Color::Yellow);

        if (entry.score == finalScore && entry.name == playerName)
        {
            scoreText.setFillColor(sf::Color::Green);
        }

        bounds = scoreText.getLocalBounds();

        scoreText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
        scoreText.setPosition((float)this->w / 2.f, currentY);

        w.draw(scoreText);

        currentY += 40.f;
    }
}

void MenuState::drawNameEntry(sf::RenderWindow& w)
{
    w.draw(titleText);

    w.draw(namePrompt);

    w.draw(inputField);

    sf::Text instructions("Press ENTER to continue", font, 30);

    instructions.setFillColor(sf::Color(150, 150, 150));

    sf::FloatRect bounds = instructions.getLocalBounds();

    instructions.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    instructions.setPosition((float)this->w / 2.f, (float)this->h / 2.f + 100.f);

    w.draw(instructions);
}