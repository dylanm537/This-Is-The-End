#include "Game.hpp"
#include <iostream>
#include "MenuState.hpp" //included for sending initial state

// constructor
Game::Game() :
    // creating sfml window with dimensions
    window(
        sf::VideoMode(Parameters::game_width, Parameters::game_height),
        "This Is The End?"
    )
{
    // makes framerate limited to 60 for 1080p@60fps
    window.setFramerateLimit(60);
    // hides cursor because we have our own crosshair
    window.setMouseCursorVisible(false);

    // loads crosshair image
    if (!crosshairTex.loadFromFile("assets/crosshair.png")) {
        std::cout << "crosshair didnt load\n";
    }
    // makes the origin of the image in the center of it
    crosshair.setTexture(crosshairTex);

    crosshair.setOrigin(crosshairTex.getSize().x / 2, crosshairTex.getSize().y / 2);
    crosshair.setScale(0.1f, 0.1f);

    // sends initial state
    pushState(std::make_unique<MenuState>(Parameters::game_width, Parameters::game_height, "assets/arial.ttf"));
}

// adds a new state to the top of the state stack, new state will be the active one
void Game::pushState(std::unique_ptr<State> state)
{
    states.push(std::move(state));
}
// removes state from top of stack, the one beneath will be the active one
void Game::popState()
{
    if (!states.empty())
    {
        states.pop();
    }
}
// changes the top state
void Game::changeState(std::unique_ptr<State> state)
{
    popState();
    pushState(std::move(state));
}

// main game loop
void Game::run()
{
    sf::Clock clock; // time between frames dt

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) // handles events
        {
            if (event.type == sf::Event::Closed)
                window.close();
            // passes event to state if there is a current state
            if (!states.empty())
            {
                states.top()->handleInput(*this, event);
            }
        }
        // updating
        if (!states.empty())
        {
            float dt = clock.restart().asSeconds();
            states.top()->update(*this, dt);
        }
        // rendering
        if (!states.empty())
        {
            window.clear(sf::Color::Black); // clears window black
            // renders current active state
            states.top()->render(window);

            // draws crosshair
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            crosshair.setPosition((float)mp.x, (float)mp.y);

            window.draw(crosshair);

            window.display();
        }
        else // if no states in the stack
        {
            window.close();
        }
    }
}