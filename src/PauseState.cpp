#include "PauseState.hpp"
#include "Game.hpp" // Required for Game::popState()

PauseState::PauseState(unsigned int windowW, unsigned int windowH, const std::string& fontPath)
{
    if (!font.loadFromFile(fontPath)) {
        // Log an error if the font fails to load, or use a default
        // In a real project, you might throw an exception here.
    }

    // 1. Setup PAUSE text
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(80);
    pauseText.setFillColor(sf::Color::Yellow);

    // Center the PAUSE text
    sf::FloatRect bounds = pauseText.getLocalBounds();
    pauseText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    pauseText.setPosition(windowW / 2.f, windowH / 2.f - 50.f);

    // 2. Setup Instructions text
    instructionsText.setFont(font);
    instructionsText.setString("Press ESC to continue");
    instructionsText.setCharacterSize(30);
    instructionsText.setFillColor(sf::Color::White);

    // Center the instruction text
    bounds = instructionsText.getLocalBounds();
    instructionsText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    instructionsText.setPosition(windowW / 2.f, windowH / 2.f + 50.f);
}

void PauseState::handleInput(Game& game, const sf::Event& event)
{
    // Check for the window close event while paused
    if (event.type == sf::Event::Closed)
    {
        game.getWindow().close();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        // Unpause: pop this state, returning to the PlayingState below it
        game.popState();
    }
}

void PauseState::update(Game& /*game*/, float /*dt*/)
{
    // The crucial step: by doing nothing in update, we freeze the game logic 
    // underneath this state.
}

void PauseState::render(sf::RenderWindow& window)
{
    // Add a dark, translucent overlay to visually pause the game screen
    sf::RectangleShape overlay({ (float)window.getSize().x, (float)window.getSize().y });
    // This color makes the screen dark but still shows the game state below
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);

    window.draw(pauseText);
    window.draw(instructionsText);
}