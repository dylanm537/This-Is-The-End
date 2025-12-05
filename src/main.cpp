#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "game_system.hpp"
#include "game_parameters.hpp"
#include <iostream>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(Parameters::game_width, Parameters::game_height),
        "This Is The End?"
    );

    sf::Texture crosshairTex;
    if (!crosshairTex.loadFromFile("assets/crosshair.png")) {
        std::cout << "FAILED TO LOAD CROSSHAIR\n";
    }

    sf::Sprite crosshair(crosshairTex);
    crosshair.setOrigin(crosshairTex.getSize().x / 2, crosshairTex.getSize().y / 2);

    crosshair.setScale(0.075f, 0.075f);

    // hide cursor
    window.setMouseCursorVisible(false);


    window.setFramerateLimit(60);

    // Menu (no background image needed)
    Menu menu(Parameters::game_width, Parameters::game_height, "assets/arial.ttf");

    // Game System
    GameSystem game;
    game.linkMenu(&menu);
    game.init();

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // NAME ENTRY INPUT HANDLED HERE

            if (menu.state == GameState::NameEntry)
            {
                if (event.type == sf::Event::TextEntered)
                {
                    // A–Z lowercase
                    if (event.text.unicode >= 'a' && event.text.unicode <= 'z')
                    {
                        if (menu.playerName.size() < 12)
                            menu.playerName += std::toupper(event.text.unicode);
                    }
                    // A–Z uppercase
                    else if (event.text.unicode >= 'A' && event.text.unicode <= 'Z')
                    {
                        if (menu.playerName.size() < 12)
                            menu.playerName += event.text.unicode;
                    }
                    // Backspace
                    else if (event.text.unicode == 8)
                    {
                        if (!menu.playerName.empty())
                            menu.playerName.pop_back();
                    }
                }
            }
        }

        float dt = clock.restart().asSeconds();

        // START SCREEN

        if (menu.state == GameState::Start)
        {
            menu.drawStart(window);

            sf::Vector2i mp = sf::Mouse::getPosition(window);
            crosshair.setPosition((float)mp.x, (float)mp.y);
            window.draw(crosshair);

            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                menu.state = GameState::NameEntry;

            continue;
        }


        // NAME ENTRY SCREEN

        if (menu.state == GameState::NameEntry)
        {
            if (!menu.playerName.empty() &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                menu.state = GameState::Playing;
                game.reset();
            }

            menu.drawNameEntry(window);

            sf::Vector2i mp = sf::Mouse::getPosition(window);
            crosshair.setPosition((float)mp.x, (float)mp.y);
            window.draw(crosshair);

            window.display();

            continue;
        }


        // GAME OVER SCREEN

        if (menu.state == GameState::GameOver)
        {
            menu.drawGameOver(window, game.getScore());

            sf::Vector2i mp = sf::Mouse::getPosition(window);
            crosshair.setPosition((float)mp.x, (float)mp.y);
            window.draw(crosshair);

            window.display();

            // Restart
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                menu.state = GameState::Start;
                menu.playerName.clear();
            }

            // Quit
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();

            continue;
        }


        // PLAYING THE GAME

        if (menu.state == GameState::Playing)
        {
            game.update(dt, window);

            if (game.isGameOver())
            {
                menu.state = GameState::GameOver;
                continue;
            }

            game.render(window);
            // update crosshiar
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            crosshair.setPosition((float)mp.x, (float)mp.y);

            // Draw crosshair LAST
            window.draw(crosshair);

            window.display();
        }
    }

    return 0;
}