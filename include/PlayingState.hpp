#pragma once
#include "State.hpp"
#include "game_system.hpp"
#include <SFML/Audio.hpp> 

class MenuState;
// playing state 
// runs the actuall game loop
class PlayingState : public State
{
public:
    PlayingState(MenuState& menu);

    ~PlayingState() override;

    void handleInput(Game& game, const sf::Event& event) override;

    void update(Game& game, float dt) override;

    void render(sf::RenderWindow& window) override;

private:
    GameSystem gameSystem; // has gamesystem which manages player, zombies, walls, scoring

    MenuState& menuStateRef; // allows returning score to menu

    sf::Music backgroundMusic; // looping gameplay music 
};