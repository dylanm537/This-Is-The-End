#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Event.hpp>   // <-- important

class Menu
{
public:
    Menu();

    // returns true if the player chooses to start the game
    bool handleEvent(const sf::Event& event);

    void draw(sf::RenderWindow& window);

    void setBackgroundTexture(const sf::Texture* texture);
    void setFont(const sf::Font* font);

private:
    const sf::Texture* m_backgroundTexture;
    sf::Sprite         m_backgroundSprite;

    const sf::Font* m_font;
    sf::Text           m_title;
    sf::Text           m_prompt;
};
