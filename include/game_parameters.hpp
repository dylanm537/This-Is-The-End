#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <algorithm>

#include "Menu.hpp"
#include "Player.hpp"
#include "Zombie.hpp"

class GameSystem
{
public:
    GameSystem();
    void run();

private:
    enum State
    {
        State_Menu,
        State_Playing,
        State_GameOver
    };

    struct Bullet
    {
        sf::CircleShape shape;
        sf::Vector2f    velocity;
    };

    void processEvents();
    void update(float dt);
    void render();

    void resetGame();
    void spawnZombie();
    void handleShooting(float dt);
    void updateBullets(float dt);
    void handleCollisions();

private:
    sf::RenderWindow m_window;
    State            m_state;

    Menu   m_menu;
    Player m_player;

    std::vector<Zombie> m_zombies;
    std::vector<Bullet> m_bullets;

    sf::Font    m_font;
    sf::Texture m_backgroundTexture;
    sf::Sprite  m_backgroundSprite;

    sf::Text m_hudText;
    sf::Text m_gameOverText;

    unsigned int m_score;
    unsigned int m_level;        // level: 0,1,2,3...
    float        m_timeSurvived;
    float        m_spawnTimer;
    float        m_fireCooldownTimer;

    sf::Clock    m_clock;
};
