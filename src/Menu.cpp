// Menu.cpp
#include "Menu.hpp"
#include <iostream>

Menu::Menu(unsigned int windowW, unsigned int windowH, sf::Texture& bgTexture, const std::string& fontPath)
: state(GameState::Start), w(windowW), h(windowH)
{
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Warning: Failed to load font at " << fontPath << ". Text may not appear.\n";
    }
    bgSprite.setTexture(bgTexture);
    sf::FloatRect b = bgSprite.getLocalBounds();
    if (b.width > 0 && b.height > 0) {
        bgSprite.setScale(float(w)/b.width, float(h)/b.height);
    }
}

void Menu::drawStart(sf::RenderWindow& window) {
    window.clear();
    window.draw(bgSprite);
    if (font.getInfo().family != "") {
        sf::Text title("THIS IS THE END", font, 48);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);
        title.setPosition(w/2.f - title.getGlobalBounds().width/2.f, h/2.f - 80.f);
        window.draw(title);

        sf::Text info("Press ENTER to Start    ESC to Quit", font, 20);
        info.setFillColor(sf::Color(200,200,200));
        info.setPosition(w/2.f - info.getGlobalBounds().width/2.f, h/2.f);
        window.draw(info);
    }
    window.display();
}

void Menu::drawGameOver(sf::RenderWindow& window, int score) {
    window.clear();
    window.draw(bgSprite);
    if (font.getInfo().family != "") {
        sf::Text go("GAME OVER", font, 48);
        go.setFillColor(sf::Color(255,50,50));
        go.setStyle(sf::Text::Bold);
        go.setPosition(w/2.f - go.getGlobalBounds().width/2.f, h/2.f - 80.f);
        window.draw(go);

        sf::Text sc(("Score: " + std::to_string(score)), font, 28);
        sc.setFillColor(sf::Color::White);
        sc.setPosition(w/2.f - sc.getGlobalBounds().width/2.f, h/2.f - 10.f);
        window.draw(sc);

        sf::Text rep("Press ENTER to Restart    ESC to Quit", font, 18);
        rep.setFillColor(sf::Color(200,200,200));
        rep.setPosition(w/2.f - rep.getGlobalBounds().width/2.f, h/2.f + 40.f);
        window.draw(rep);
    }
    window.display();
}
