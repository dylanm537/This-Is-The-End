#include "game_system.hpp"
#include "game_parameters.hpp"
#include "Player.hpp"
#include "Zombie.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

using param = Parameters;

// Shared spritesheet
sf::Texture GameSystem::spritesheet;

// Internal game data
namespace
{
    std::unique_ptr<Player> g_player;
    std::vector<sf::RectangleShape> g_walls;
    std::vector<Zombie> g_zombies;

    float g_zombieSpawnTimer = 0.f;
    float g_zombieSpawnInterval = 5.f;

    float g_difficultyTimer = 0.f;
    int g_zombiesPerWave = 3;

    const float g_difficultyIncreaseInterval = 50.f;

    float g_damageCooldown = 0.f;
    const float g_damageInterval = 0.4f;

    int g_score = 0;

    bool g_isGameOver = false; 
}

// Helper to make a texture
static sf::Texture createSolidTexture(unsigned size, sf::Color c)
{
    sf::Image img;
    img.create(size, size, c);
    sf::Texture t;
    t.loadFromImage(img);
    return t;
}

static void spawnZombies(int count)
{
    for (int i = 0; i < count; i++)
    {
        float x = param::game_width + 40.f;
        float y = 50 + std::rand() % (param::game_height - 100);
        g_zombies.emplace_back(x, y, GameSystem::spritesheet);
    }
}

void GameSystem::init()
{
    if (spritesheet.getSize().x == 0)
        spritesheet = createSolidTexture(32, sf::Color::Cyan);

    g_player = std::make_unique<Player>(100.f, 100.f, spritesheet);

    // walls
    g_walls.clear();
    auto makeWall = [](sf::Vector2f pos, sf::Vector2f size)
    {
        sf::RectangleShape w(size);
        w.setOrigin(size.x / 2, size.y / 2);
        w.setFillColor(sf::Color(80, 80, 80));
        w.setPosition(pos);
        return w;
    };

    g_walls.push_back(makeWall({ 400,300 }, { 200,40 }));
    g_walls.push_back(makeWall({ 200,200 }, { 40,180 }));
    g_walls.push_back(makeWall({ 600,400 }, { 40,220 }));

    // Reset game content
    g_zombies.clear();
    g_zombieSpawnTimer = 0.f;
    g_difficultyTimer = 0.f;
    g_zombiesPerWave = 3;
    g_damageCooldown = 0.f;

    g_score = 0;
    g_isGameOver = false;
}

void GameSystem::reset()
{
    init();
}

void GameSystem::clean()
{
    g_player.reset();
    g_zombies.clear();
    g_walls.clear();
}

int GameSystem::getScore() const
{
    return g_score;
}

bool GameSystem::isGameOver() const
{
    return g_isGameOver;
}

void GameSystem::update(const float& dt, const sf::RenderWindow& window)
{
    g_damageCooldown -= dt;

    sf::Vector2f prevPos = g_player->getCenter();
    g_player->handleInput(window, dt);
    g_player->update(dt);
    g_player->updateBullets(dt);

    // Player vs. walls
    sf::FloatRect pBounds = g_player->getBounds();
    for (auto& w : g_walls)
    {
        if (pBounds.intersects(w.getGlobalBounds()))
        {
            g_player->moveBy(prevPos - g_player->getCenter());
            break;
        }
    }

    // Update zombies
    sf::Vector2f p = g_player->getCenter();
    for (auto& z : g_zombies)
        z.update(p, dt, g_walls);

    // Damage
    if (g_damageCooldown <= 0.f)
    {
        for (auto& z : g_zombies)
        {
            if (!z.isDead() && pBounds.intersects(z.getBounds()))
            {
                g_player->takeDamage(1);
                g_damageCooldown = g_damageInterval;
                break;
            }
        }
    }

    // Bullet hits
    for (auto& b : g_player->getBullets())
    {
        if (!b.isAlive()) continue;

        for (auto& z : g_zombies)
        {
            if (!z.isDead() && b.getBounds().intersects(z.getBounds()))
            {
                z.damage(1);
                b.kill();

                if (z.isDead())
                    g_score++;

                break;
            }
        }
    }

    // Remove dead zombies
    g_zombies.erase(
        std::remove_if(g_zombies.begin(), g_zombies.end(),
            [](const Zombie& z) { return z.isDead(); }),
        g_zombies.end()
    );

    // Difficulty increase
    g_difficultyTimer += dt;
    if (g_difficultyTimer >= g_difficultyIncreaseInterval)
    {
        g_difficultyTimer = 0.f;
        g_zombiesPerWave += 3;
    }

    // Spawning waves
    g_zombieSpawnTimer += dt;
    if (g_zombieSpawnTimer >= g_zombieSpawnInterval)
    {
        g_zombieSpawnTimer = 0.f;
        spawnZombies(g_zombiesPerWave);
    }

   
    if (g_player->getHP() <= 0)
    {
        g_isGameOver = true;
    }
}

void GameSystem::render(sf::RenderWindow& window)
{
    window.clear(sf::Color(25, 25, 40));

    for (auto& w : g_walls)
        window.draw(w);

    for (auto& z : g_zombies)
        z.draw(window);

    g_player->draw(window);

    // HUD
    sf::Font hudFont;
    hudFont.loadFromFile("assets/arial.ttf");

    sf::Text hp("HP: " + std::to_string(g_player->getHP()), hudFont, 18);
    hp.setFillColor(sf::Color::White);
    hp.setPosition(10, 10);
    window.draw(hp);

    sf::Text sc("Score: " + std::to_string(g_score), hudFont, 18);
    sc.setFillColor(sf::Color::Cyan);
    sc.setPosition(param::game_width - 120, 10);
    window.draw(sc);
}