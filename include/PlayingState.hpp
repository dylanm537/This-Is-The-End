#pragma once
#include "State.hpp"
#include "game_system.hpp"
#include <SFML/Audio.hpp> // <-- Add this include

class MenuState;

class PlayingState : public State
{
public:
    PlayingState(MenuState& menu);

    void handleInput(Game& game, const sf::Event& event) override;
    void update(Game& game, float dt) override;
    void render(sf::RenderWindow& window) override;

private:
    GameSystem gameSystem;
    MenuState& menuStateRef;

    // NEW: Background Music Object
    sf::Music backgroundMusic;
};