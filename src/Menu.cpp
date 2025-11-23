#include "Menu.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Menu::Menu(unsigned int windowW, unsigned int windowH, sf::Texture& bgTexture, const std::string& fontPath)
    : state(GameState::Start)
    , w(windowW)
    , h(windowH)
{
    if (!fontPath.empty())
    {
        if (!font.loadFromFile(fontPath))
        {
            std::cerr << "Warning: Failed to load font at " << fontPath
                << ". Menu text may not appear.\n";
        }
    }

    bgSprite.setTexture(bgTexture);
    sf::FloatRect b = bgSprite.getLocalBounds();
    if (b.width > 0.f && b.height > 0.f)
    {
        bgSprite.setScale(static_cast<float>(w) / b.width,
            static_cast<float>(h) / b.height);
    }
}

void Menu::drawStart(sf::RenderWindow& window)
{
    window.clear();

    if (bgSprite.getTexture())
    {
        window.draw(bgSprite);
    }
    else
    {
        sf::RectangleShape bg(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
        bg.setFillColor(sf::Color(20, 20, 40));
        window.draw(bg);
    }

    if (font.getInfo().family != "")
    {
        sf::Text title("THIS IS THE END?", font, 52);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        title.setPosition(
            static_cast<float>(w) / 2.f - title.getGlobalBounds().width / 2.f,
            static_cast<float>(h) / 2.f - 120.f
        );
        window.draw(title);

        sf::Text info("Press ENTER to Start\nPress ESC to Quit", font, 22);
        info.setFillColor(sf::Color(200, 200, 220));
        info.setPosition(
            static_cast<float>(w) / 2.f - info.getGlobalBounds().width / 2.f,
            static_cast<float>(h) / 2.f
        );
        window.draw(info);
    }

    window.display();
}

void Menu::drawGameOver(sf::RenderWindow& window, int score)
{
    window.clear();

    if (bgSprite.getTexture())
    {
        window.draw(bgSprite);
    }
    else
    {
        sf::RectangleShape bg(sf::Vector2f(static_cast<float>(w), static_cast<float>(h)));
        bg.setFillColor(sf::Color(40, 10, 10));
        window.draw(bg);
    }

    if (font.getInfo().family != "")
    {
        sf::Text go("GAME OVER", font, 52);
        go.setFillColor(sf::Color(255, 60, 60));
        go.setStyle(sf::Text::Bold);
        go.setPosition(
            static_cast<float>(w) / 2.f - go.getGlobalBounds().width / 2.f,
            static_cast<float>(h) / 2.f - 140.f
        );
        window.draw(go);

        sf::Text sc("Score: " + std::to_string(score), font, 28);
        sc.setFillColor(sf::Color::White);
        sc.setPosition(
            static_cast<float>(w) / 2.f - sc.getGlobalBounds().width / 2.f,
            static_cast<float>(h) / 2.f - 20.f
        );
        window.draw(sc);

        sf::Text rep("Press ENTER to Restart    ESC to Quit", font, 18);
        rep.setFillColor(sf::Color(230, 230, 230));
        rep.setPosition(
            static_cast<float>(w) / 2.f - rep.getGlobalBounds().width / 2.f,
            static_cast<float>(h) / 2.f + 40.f
        );
        window.draw(rep);
    }

    window.display();
}