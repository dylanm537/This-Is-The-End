#include "game_system.hpp"
#include "game_parameters.hpp"
#include "Player.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using param = Parameters;

sf::Texture GameSystem::spritesheet;

namespace
{
    std::unique_ptr<Player> g_player;
    std::vector<sf::RectangleShape> g_walls;

    sf::Font g_hudFont;
    bool     g_hudFontLoaded = false;

    int      g_score = 0;
}

static sf::Texture createSolidTexture(unsigned int size, sf::Color color)
{
    sf::Image img;
    img.create(size, size, color);

    sf::Texture tex;
    tex.loadFromImage(img);
    return tex;
}

void GameSystem::init()
{
    if (spritesheet.getSize().x == 0 || spritesheet.getSize().y == 0)
    {
        spritesheet = createSolidTexture(32, sf::Color::Cyan);
    }

    g_player = std::make_unique<Player>(
        100.f,
        100.f,
        spritesheet
        );

    g_walls.clear();

    auto makeWall = [](sf::Vector2f pos, sf::Vector2f size, sf::Color color)
    {
        sf::RectangleShape wall(size);
        wall.setFillColor(color);
        wall.setOrigin(size.x / 2.f, size.y / 2.f);
        wall.setPosition(pos);
        return wall;
    };

    g_walls.push_back(makeWall({ 400.f, 300.f }, { 200.f, 40.f }, sf::Color(80, 80, 80)));
    g_walls.push_back(makeWall({ 200.f, 200.f }, { 40.f, 180.f }, sf::Color(90, 90, 90)));
    g_walls.push_back(makeWall({ 600.f, 400.f }, { 40.f, 220.f }, sf::Color(90, 90, 90)));

    if (g_hudFont.loadFromFile("assets/arial.ttf"))
    {
        g_hudFontLoaded = true;
    }
    else
    {
        g_hudFontLoaded = false;
        std::cerr << "Warning: could not load HUD font at assets/arial.ttf. HUD text will not be shown.\n";
    }

    g_score = 0;
}

void GameSystem::reset()
{
    init();
}

void GameSystem::clean()
{
    g_player.reset();
    g_walls.clear();
}

int GameSystem::getScore()
{
    return g_score;
}

void GameSystem::update(const float& dt, const sf::RenderWindow& window)
{
    if (!g_player)
        return;

    sf::Vector2f previousPos = g_player->getCenter();

    g_player->handleInput(window, dt);
    g_player->update(dt);
    g_player->updateBullets(dt);

    sf::FloatRect playerBounds = g_player->getBounds();

    for (const auto& wall : g_walls)
    {
        if (playerBounds.intersects(wall.getGlobalBounds()))
        {
            // move player back by difference
            sf::Vector2f currentPos = g_player->getCenter();
            sf::Vector2f correction = previousPos - currentPos;
            g_player->moveBy(correction);
            break;
        }
    }
}

void GameSystem::render(sf::RenderWindow& window)
{
    if (!g_player)
        return;

    window.clear(sf::Color(25, 25, 40));

    for (const auto& wall : g_walls)
    {
        window.draw(wall);
    }

    g_player->draw(window);

    if (g_hudFontLoaded)
    {
        sf::Text help;
        help.setFont(g_hudFont);
        help.setCharacterSize(16);
        help.setFillColor(sf::Color::White);
        help.setString("WASD: Move    Mouse: Aim    LMB: Shoot");
        help.setPosition(10.f, static_cast<float>(param::game_height) - 30.f);
        window.draw(help);

        sf::Text hp;
        hp.setFont(g_hudFont);
        hp.setCharacterSize(18);
        hp.setFillColor(sf::Color::White);
        hp.setString("HP: " + std::to_string(g_player->getHP()));
        hp.setPosition(10.f, 10.f);
        window.draw(hp);
    }

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    const float cs = 8.f;
    sf::Vertex lines[4] =
    {
        sf::Vertex(sf::Vector2f(mouseWorld.x - cs, mouseWorld.y), sf::Color::White),
        sf::Vertex(sf::Vector2f(mouseWorld.x + cs, mouseWorld.y), sf::Color::White),
        sf::Vertex(sf::Vector2f(mouseWorld.x, mouseWorld.y - cs), sf::Color::White),
        sf::Vertex(sf::Vector2f(mouseWorld.x, mouseWorld.y + cs), sf::Color::White),
    };
    window.draw(lines, 4, sf::Lines);
}