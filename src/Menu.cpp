#include "Menu.hpp"
#include "game_parameters.hpp"

#include <cstdlib>   // std::exit

Menu::Menu()
    : m_backgroundTexture(nullptr)
    , m_font(nullptr)
{
}

void Menu::setBackgroundTexture(const sf::Texture* texture)
{
    m_backgroundTexture = texture;

    if (m_backgroundTexture)
    {
        m_backgroundSprite.setTexture(*m_backgroundTexture);

        // Scale background to window size
        sf::Vector2u texSize = m_backgroundTexture->getSize();
        if (texSize.x != 0 && texSize.y != 0)
        {
            float scaleX = static_cast<float>(GameParameters::WindowWidth) / texSize.x;
            float scaleY = static_cast<float>(GameParameters::WindowHeight) / texSize.y;
            m_backgroundSprite.setScale(scaleX, scaleY);
        }
    }
}

void Menu::setFont(const sf::Font* font)
{
    m_font = font;

    if (!m_font)
        return;

    m_title.setFont(*m_font);
    m_title.setString("THIS IS THE END");
    m_title.setCharacterSize(64);
    m_title.setFillColor(sf::Color::White);
    m_title.setPosition(100.f, 150.f);

    m_prompt.setFont(*m_font);
    m_prompt.setString("Press ENTER to play\nPress ESC to quit");
    m_prompt.setCharacterSize(32);
    m_prompt.setFillColor(sf::Color::White);
    m_prompt.setPosition(100.f, 300.f);
}

bool Menu::handleEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Enter)
        {
            return true; // start game
        }
        else if (event.key.code == sf::Keyboard::Escape)
        {
            std::exit(0);
        }
    }

    return false;
}

void Menu::draw(sf::RenderWindow& window)
{
    if (m_backgroundTexture)
        window.draw(m_backgroundSprite);

    if (m_font)
    {
        window.draw(m_title);
        window.draw(m_prompt);
    }
}
