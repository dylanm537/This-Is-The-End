#include "PauseState.hpp"
#include "Game.hpp" 

PauseState::PauseState(unsigned int windowW, unsigned int windowH, const std::string& fontPath)
{
    if (!font.loadFromFile(fontPath)) {
        
    }

    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(80);
    pauseText.setFillColor(sf::Color::Yellow);

    sf::FloatRect bounds = pauseText.getLocalBounds();

    pauseText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    pauseText.setPosition(windowW / 2.f, windowH / 2.f - 50.f);
  
    instructionsText.setFont(font);
    instructionsText.setString("Press ESC to continue");
    instructionsText.setCharacterSize(30);
    instructionsText.setFillColor(sf::Color::White);
   
    bounds = instructionsText.getLocalBounds();

    instructionsText.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    instructionsText.setPosition(windowW / 2.f, windowH / 2.f + 50.f);
}

void PauseState::handleInput(Game& game, const sf::Event& event)
{
    
    if (event.type == sf::Event::Closed)
    {
        game.getWindow().close();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        
        game.popState();
    }
}

void PauseState::update(Game&, float )
{
    
}

void PauseState::render(sf::RenderWindow& window)
{   
    sf::RectangleShape overlay({ (float)window.getSize().x, (float)window.getSize().y });
    
    overlay.setFillColor(sf::Color(0, 0, 0, 150));

    window.draw(overlay);

    window.draw(pauseText);
    window.draw(instructionsText);
}