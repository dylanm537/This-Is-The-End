#include "Game.hpp"
#include <iostream>
#include "MenuState.hpp"

Game::Game() :
    window(
        sf::VideoMode(Parameters::game_width, Parameters::game_height),
        "This Is The End?"
    )
{
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);

    // Initialize Crosshair
    if (!crosshairTex.loadFromFile("assets/crosshair.png")) {
        std::cout << "FAILED TO LOAD CROSSHAIR\n";
    }
    crosshair.setTexture(crosshairTex);
    crosshair.setOrigin(crosshairTex.getSize().x / 2, crosshairTex.getSize().y / 2);
    crosshair.setScale(0.1f, 0.1f);

    // Start with the MenuState
    pushState(std::make_unique<MenuState>(Parameters::game_width, Parameters::game_height, "assets/arial.ttf"));
}

void Game::pushState(std::unique_ptr<State> state)
{
    states.push(std::move(state));
}

void Game::popState()
{
    if (!states.empty())
    {
        states.pop();
    }
}

void Game::changeState(std::unique_ptr<State> state)
{
    popState();
    pushState(std::move(state));
}

void Game::run()
{
    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!states.empty())
            {
                states.top()->handleInput(*this, event);
            }
        }

        if (!states.empty())
        {
            float dt = clock.restart().asSeconds();
            states.top()->update(*this, dt);
        }

        if (!states.empty())
        {
            window.clear(sf::Color::Black);
            states.top()->render(window);

            // Draw crosshair on top of everything
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            crosshair.setPosition((float)mp.x, (float)mp.y);
            window.draw(crosshair);

            window.display();
        }
        else // No more states, quit
        {
            window.close();
        }
    }
}