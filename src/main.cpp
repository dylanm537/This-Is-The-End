#include <SFML/Graphics.hpp>

#include "game_parameters.hpp"
#include "game_system.hpp"
#include "Menu.hpp"

using param = Parameters;

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(param::game_width, param::game_height),
        "This Is The End?"
    );
    window.setFramerateLimit(60);

    sf::Texture menuBgTexture;
    {
        sf::Image img;
        img.create(1, 1, sf::Color(10, 10, 30));
        menuBgTexture.loadFromImage(img);
    }

    Menu menu(param::game_width, param::game_height, menuBgTexture, "assets/arial.ttf");

    GameSystem::init();

    GameState state = GameState::Start;

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (state == GameState::Start)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        GameSystem::reset();
                        state = GameState::Playing;
                    }
                    else if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }
                }
                else if (state == GameState::Playing)
                {                   
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        state = GameState::GameOver;
                    }
                }
                else if (state == GameState::GameOver)
                {
                    if (event.key.code == sf::Keyboard::Enter)
                    {
                        GameSystem::reset();
                        state = GameState::Playing;
                    }
                    else if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }
                }
            }
        }

        if (state == GameState::Start)
        {
            menu.drawStart(window);
        }
        else if (state == GameState::Playing)
        {
            GameSystem::update(dt, window);
            GameSystem::render(window);
            window.display();
        }
        else if (state == GameState::GameOver)
        {
            menu.drawGameOver(window, GameSystem::getScore());
        }
    }

    GameSystem::clean();
    return 0;
}