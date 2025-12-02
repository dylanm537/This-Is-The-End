#include "game_system.hpp"
#include "game_parameters.hpp"
#include "Player.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/System/Time.hpp>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <cmath>    // for std::sqrt

<<<<<<< Updated upstream
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
=======
GameSystem::GameSystem()
    : m_window(
        sf::VideoMode(GameParameters::WindowWidth, GameParameters::WindowHeight),
        GameParameters::WindowTitle)
    , m_state(State_Menu)
    , m_score(0)
    , m_level(0)          // <-- NEW: start at level 0
    , m_timeSurvived(0.f)
    , m_spawnTimer(0.f)
    , m_fireCooldownTimer(0.f)
{
    m_window.setFramerateLimit(60);

    // Seed RNG
    std::srand(static_cast<unsigned int>(std::time(0)));

    // Load font
    if (!m_font.loadFromFile("assets/yourfont.ttf"))
    {
        // If this fails, text will just not show properly
    }

    // Load background if present
    if (m_backgroundTexture.loadFromFile("assets/background.png"))
    {
        m_backgroundSprite.setTexture(m_backgroundTexture);
        sf::Vector2u texSize = m_backgroundTexture.getSize();
        float scaleX = static_cast<float>(GameParameters::WindowWidth) / texSize.x;
        float scaleY = static_cast<float>(GameParameters::WindowHeight) / texSize.y;
        m_backgroundSprite.setScale(scaleX, scaleY);
    }

    // Setup menu
    m_menu.setBackgroundTexture(&m_backgroundTexture);
    m_menu.setFont(&m_font);

    // HUD
    m_hudText.setFont(m_font);
    m_hudText.setCharacterSize(24);
    m_hudText.setFillColor(sf::Color::White);
    m_hudText.setPosition(10.f, 10.f);

    m_gameOverText.setFont(m_font);
    m_gameOverText.setCharacterSize(40);
    m_gameOverText.setFillColor(sf::Color::Red);
    m_gameOverText.setString("GAME OVER\nPress ENTER to restart\nPress ESC to quit");
    m_gameOverText.setPosition(200.f, 250.f);

    resetGame();
}

void GameSystem::run()
{
    while (m_window.isOpen())
    {
        float dt = m_clock.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}

void GameSystem::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();

        switch (m_state)
        {
        case State_Menu:
            if (m_menu.handleEvent(event))
            {
                resetGame();
                m_state = State_Playing;
            }
            break;

        case State_Playing:
            m_player.handleEvent(event);
            break;

        case State_GameOver:
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Enter)
                {
                    resetGame();
                    m_state = State_Playing;
                }
                else if (event.key.code == sf::Keyboard::Escape)
                {
                    m_window.close();
                }
            }
            break;
        }
    }
}

void GameSystem::update(float dt)
{
    if (m_state == State_Playing)
    {
        m_timeSurvived += dt;

        // --- NEW: simple level system, +1 level every 10 seconds ---
        m_level = static_cast<unsigned int>(m_timeSurvived / 10.f);

        m_spawnTimer += dt;
        m_fireCooldownTimer -= dt;
        if (m_fireCooldownTimer < 0.f)
            m_fireCooldownTimer = 0.f;

        m_player.update(dt, m_window);
        m_player.updateInvulnerability(dt);

        if (m_spawnTimer >= GameParameters::ZombieSpawnInterval)
        {
            spawnZombie();
            m_spawnTimer = 0.f;
        }

        handleShooting(dt);
        updateBullets(dt);

        // Update zombies
        for (std::size_t i = 0; i < m_zombies.size(); ++i)
        {
            m_zombies[i].update(dt, m_player.getPosition());
        }

        handleCollisions();

        if (m_player.isDead())
        {
            m_state = State_GameOver;
        }

        // Update HUD text (now includes Level)
        std::ostringstream ss;
        ss << "Lives: " << m_player.getLives()
            << "    Score: " << m_score
            << "    Level: " << m_level
            << "    Time: " << static_cast<int>(m_timeSurvived);
        m_hudText.setString(ss.str());
    }
}

void GameSystem::render()
{
    m_window.clear();

    if (m_state == State_Menu)
    {
        if (m_backgroundTexture.getSize().x != 0)
            m_window.draw(m_backgroundSprite);

        m_menu.draw(m_window);
    }
    else if (m_state == State_Playing)
    {
        if (m_backgroundTexture.getSize().x != 0)
            m_window.draw(m_backgroundSprite);

        // Draw bullets behind player so they look clean
        for (std::size_t i = 0; i < m_bullets.size(); ++i)
            m_window.draw(m_bullets[i].shape);

        // Draw zombies
        for (std::size_t i = 0; i < m_zombies.size(); ++i)
            m_zombies[i].draw(m_window);

        m_player.draw(m_window);
        m_window.draw(m_hudText);
    }
    else if (m_state == State_GameOver)
    {
        if (m_backgroundTexture.getSize().x != 0)
            m_window.draw(m_backgroundSprite);

        m_window.draw(m_gameOverText);
        m_window.draw(m_hudText);
    }

    m_window.display();
}

void GameSystem::resetGame()
{
    m_player.reset();
    m_zombies.clear();
    m_bullets.clear();
    m_score = 0;
    m_level = 0;          // <-- NEW: reset level
    m_timeSurvived = 0.f;
    m_spawnTimer = 0.f;
    m_fireCooldownTimer = 0.f;

    m_clock.restart();
}

void GameSystem::spawnZombie()
{
    // Spawn somewhere just off-screen around the edges
    float x = 0.f;
    float y = 0.f;

    int side = std::rand() % 4; // 0=top,1=bottom,2=left,3=right

    switch (side)
    {
    case 0: // top
        x = static_cast<float>(std::rand() % GameParameters::WindowWidth);
        y = -GameParameters::ZombieRadius * 2.f;
        break;
    case 1: // bottom
        x = static_cast<float>(std::rand() % GameParameters::WindowWidth);
        y = GameParameters::WindowHeight + GameParameters::ZombieRadius * 2.f;
        break;
    case 2: // left
        x = -GameParameters::ZombieRadius * 2.f;
        y = static_cast<float>(std::rand() % GameParameters::WindowHeight);
        break;
    case 3: // right
        x = GameParameters::WindowWidth + GameParameters::ZombieRadius * 2.f;
        y = static_cast<float>(std::rand() % GameParameters::WindowHeight);
        break;
    }

    float t = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
    float speed = GameParameters::ZombieMinSpeed +
        t * (GameParameters::ZombieMaxSpeed - GameParameters::ZombieMinSpeed);

    Zombie z;
    z.spawn(sf::Vector2f(x, y), speed);
    m_zombies.push_back(z);
}

void GameSystem::handleShooting(float /*dt*/)
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
        m_fireCooldownTimer <= 0.f)
    {
        Bullet b;
        b.shape.setRadius(GameParameters::BulletRadius);
        b.shape.setOrigin(GameParameters::BulletRadius, GameParameters::BulletRadius);
        b.shape.setFillColor(sf::Color::Yellow);
        b.shape.setPosition(m_player.getPosition());

        sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
        sf::Vector2f target = m_window.mapPixelToCoords(mousePos);
        sf::Vector2f dir = target - m_player.getPosition();
        float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (len > 0.f)
        {
            dir.x /= len;
            dir.y /= len;
        }
        b.velocity = dir * GameParameters::BulletSpeed;

        m_bullets.push_back(b);
        m_fireCooldownTimer = GameParameters::FireCooldown;
    }
}

void GameSystem::updateBullets(float dt)
{
    for (std::size_t i = 0; i < m_bullets.size(); ++i)
    {
        Bullet& b = m_bullets[i];
        b.shape.move(b.velocity * dt);
    }

    // Remove bullets that go off-screen
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& b)
            {
                sf::Vector2f p = b.shape.getPosition();
    return p.x < -50.f || p.y < -50.f ||
        p.x > GameParameters::WindowWidth + 50.f ||
        p.y > GameParameters::WindowHeight + 50.f;
            }),
        m_bullets.end());
}

void GameSystem::handleCollisions()
{
    // Bullets vs zombies
    for (std::size_t i = 0; i < m_bullets.size(); ++i)
    {
        sf::FloatRect bulletBounds = m_bullets[i].shape.getGlobalBounds();

        for (std::size_t j = 0; j < m_zombies.size(); ++j)
        {
            if (!m_zombies[j].isAlive())
                continue;

            sf::FloatRect zombieBounds = m_zombies[j].getShape().getGlobalBounds();

            if (bulletBounds.intersects(zombieBounds))
            {
                m_zombies[j].kill();
                m_score += 10;
                // destroy bullet
                m_bullets.erase(m_bullets.begin() + i);
                if (i > 0) --i;
                break;
            }
        }
    }

    // Remove dead zombies
    m_zombies.erase(
        std::remove_if(m_zombies.begin(), m_zombies.end(),
            [](const Zombie& z) { return !z.isAlive(); }),
        m_zombies.end());

    // Zombies vs player
    if (!m_player.isInvulnerable())
    {
        sf::FloatRect playerBounds = m_player.getShape().getGlobalBounds();

        for (std::size_t j = 0; j < m_zombies.size(); ++j)
        {
            if (!m_zombies[j].isAlive())
                continue;

            sf::FloatRect zombieBounds = m_zombies[j].getShape().getGlobalBounds();
            if (playerBounds.intersects(zombieBounds))
            {
                m_player.damage();
                if (m_player.isDead())
                    return;
                else
                    break;
            }
        }
    }
>>>>>>> Stashed changes
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