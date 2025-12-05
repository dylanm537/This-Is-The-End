#include "MenuState.hpp"
#include "Game.hpp"
#include "PlayingState.hpp"
#include <iostream>

using namespace std;

MenuState::MenuState(unsigned int windowW, unsigned int windowH, const std::string& fontPath)
    : menuState(GameState::Start)
    , w(windowW)
    , h(windowH)
    , playerName("")
{
    if (!font.loadFromFile(fontPath))
    {
        cerr << "Warning: Could not load font\n";
    }

    // Title
    titleText.setFont(font);
    titleText.setCharacterSize(52);
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    titleText.setString("THIS IS THE END?");
    titleText.setPosition(w / 2.f - titleText.getGlobalBounds().width / 2.f, 120.f);

    // Name prompt
    namePrompt.setFont(font);
    namePrompt.setCharacterSize(32);
    namePrompt.setFillColor(sf::Color::White);
    namePrompt.setString("ENTER YOUR NAME:");
    namePrompt.setPosition(w / 2.f - 180.f, 260.f);

    // Name typed text
    nameText.setFont(font);
    nameText.setCharacterSize(28);
    nameText.setFillColor(sf::Color::Yellow);
    nameText.setPosition(w / 2.f - 150.f, 310.f);
}

void MenuState::handleInput(Game& game, const sf::Event& event)
{
    // Input for Start/Game Over
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            game.popState(); // Quit the engine
            return;
        }

        if (event.key.code == sf::Keyboard::Enter)
        {
            if (menuState == GameState::Start)
            {
                menuState = GameState::NameEntry;
            }
            else if (menuState == GameState::NameEntry && !playerName.empty())
            {
                
                game.pushState(std::make_unique<PlayingState>(*this));
                menuState = GameState::Playing;
            }
            else if (menuState == GameState::GameOver)
            {
                // Go back to the Start screen
                menuState = GameState::Start;
            }
        }
    }

    // Input for Name Entry 
    if (menuState == GameState::NameEntry && event.type == sf::Event::TextEntered)
    {
        if (event.text.unicode >= 'a' && event.text.unicode <= 'z' || event.text.unicode >= 'A' && event.text.unicode <= 'Z')
        {
            if (playerName.size() < 12)
                playerName += static_cast<char>(event.text.unicode);
        }
        else if (event.text.unicode == 8) 
        {
            if (!playerName.empty())
                playerName.pop_back();
        }
    }
}

void MenuState::update(Game& /*game*/, float /*dt*/)
{
    
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
    else if (menuState == GameState::GameOver)
    {
        
        int score = (gameSystem) ? gameSystem->getScore() : 0;
        drawGameOver(window, score);
    }
}

void MenuState::drawStart(sf::RenderWindow& w)
{
    w.clear(sf::Color(15, 15, 30));
    w.draw(titleText);

    sf::Text info("Press ENTER to Continue\nPress ESC to Quit", font, 22);
    info.setFillColor(sf::Color(200, 200, 220));
    info.setPosition(w.getSize().x / 2.f - info.getGlobalBounds().width / 2.f, 350.f);

    w.draw(info);
}

void MenuState::drawNameEntry(sf::RenderWindow& w)
{
    w.clear(sf::Color(15, 15, 30));

    w.draw(namePrompt);

    nameText.setString(playerName + "_");
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
    gameOver.setPosition(w.getSize().x / 2.f - gameOver.getGlobalBounds().width / 2.f, 150.f);
    w.draw(gameOver);

    sf::Text sc("Score: " + std::to_string(score), font, 30);
    sc.setFillColor(sf::Color::White);
    sc.setPosition(w.getSize().x / 2.f - sc.getGlobalBounds().width / 2.f, 260.f);
    w.draw(sc);

    sf::Text info("Press ENTER to Restart    ESC to Quit", font, 20);
    info.setFillColor(sf::Color(230, 230, 230));
    info.setPosition(w.getSize().x / 2.f - info.getGlobalBounds().width / 2.f, 330.f);
    w.draw(info);
}