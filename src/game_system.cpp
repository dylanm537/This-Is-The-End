#include "game_system.hpp"
#include "game_parameters.hpp"
#include "Player.hpp"
#include "Zombie.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm> 
#include <random> // Required for generating random numbers

using param = Parameters;

// Shared spritesheet (Kept but not used for player/zombie)
sf::Texture GameSystem::spritesheet;

// Internal game data
namespace
{
    // Persistent Textures
    static sf::Texture g_playerTex;
    static sf::Texture g_zombieTex;

    // Map Background
    static sf::Texture g_mapTex;
    static sf::Sprite g_mapSprite;

    // Truck Obstacle
    static sf::Texture g_truckTex;
    static std::vector<sf::Sprite> g_truckSprites;

    std::unique_ptr<Player> g_player;
    // g_walls holds the collision boxes for the truck obstacles
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

// NEW HELPER: Random float generator
static float randFloat(float a, float b) {
    // This is a robust way to generate a uniform random float
    static std::mt19937 rng((unsigned)std::random_device{}());
    std::uniform_real_distribution<float> d(a, b);
    return d(rng);
}

// Helper to make a texture (original code, kept)
static sf::Texture createSolidTexture(unsigned size, sf::Color c)
{
    sf::Image img;
    img.create(size, size, c);
    sf::Texture t;
    t.loadFromImage(img);
    return t;
}

// MODIFIED: Zombies now spawn from a randomly chosen side 
static void spawnZombies(int count)
{
    // Define how far off-screen the zombie should start
    const float offset = 40.f;

    for (int i = 0; i < count; i++)
    {
        float x, y;

        // Randomly choose a side (1=Top, 2=Bottom, 3=Left, 4=Right)
        int side = (std::rand() % 4) + 1;

        if (side == 1) // Top side
        {
            // Random X across the map width, fixed Y just above the screen
            x = randFloat(0.f, (float)param::game_width);
            y = -offset;
        }
        else if (side == 2) // Bottom side
        {
            // Random X across the map width, fixed Y just below the screen
            x = randFloat(0.f, (float)param::game_width);
            y = (float)param::game_height + offset;
        }
        else if (side == 3) // Left side
        {
            // Fixed X just left of the screen, random Y across the map height
            x = -offset;
            y = randFloat(0.f, (float)param::game_height);
        }
        else // side == 4 (Right side)
        {
            // Fixed X just right of the screen, random Y across the map height
            x = (float)param::game_width + offset;
            y = randFloat(0.f, (float)param::game_height);
        }

        g_zombies.emplace_back(x, y, g_zombieTex);
    }
}


// NEW HELPER: Creates a single truck sprite and its collision box
static void createTruckObstacle(sf::Vector2f position)
{
    // VISUAL SPRITE SCALING & POSITION 
    const float desiredWidth = 120.f;

    // COLLISION ADJUSTMENTS (Current tuned values)
    const float collisionWidth = 28.f;
    const float collisionHeight = 38.f;

    // --- 1. Create the VISUAL Sprite ---
    sf::Sprite truckSprite(g_truckTex);
    sf::FloatRect bounds = truckSprite.getLocalBounds();

    float scale = desiredWidth / bounds.width;
    truckSprite.setScale(scale, scale);
    truckSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    truckSprite.setPosition(position);

    g_truckSprites.push_back(truckSprite);

    // --- 2. Create the INVISIBLE Collision Box ---
    sf::RectangleShape truckCollision({ collisionWidth, collisionHeight });

    // Set the origin/position to match the sprite's center
    truckCollision.setOrigin(collisionWidth / 2.f, collisionHeight / 2.f);
    truckCollision.setPosition(position);

    // The collision shape is invisible
    truckCollision.setFillColor(sf::Color::Transparent);

    g_walls.push_back(truckCollision);
}


void GameSystem::init()
{
    // 1. Load Player and Zombie Textures
    if (!g_playerTex.loadFromFile("assets/player.png")) {
        std::cerr << "FATAL: FAILED TO LOAD assets/player.png\n";
    }

    if (!g_zombieTex.loadFromFile("assets/zombie.png")) {
        std::cerr << "FATAL: FAILED TO LOAD assets/zombie.png\n";
    }

    // 2. Setup Map Background (with scaling fix)
    if (!g_mapTex.loadFromFile("assets/map.png")) {
        std::cerr << "WARNING: FAILED TO LOAD assets/map.png. Using default background.\n";
    }
    else {
        g_mapSprite.setTexture(g_mapTex);

        sf::Vector2u mapSize = g_mapTex.getSize();
        float scaleX = (float)param::game_width / mapSize.x;
        float scaleY = (float)param::game_height / mapSize.y;

        g_mapSprite.setScale(scaleX, scaleY);
    }

    // 3. Setup Truck Obstacles (Randomized)
    g_walls.clear(); // Clear collision boxes
    g_truckSprites.clear(); // Clear visual sprites

    if (!g_truckTex.loadFromFile("assets/truck.png")) {
        std::cerr << "WARNING: FAILED TO LOAD assets/truck.png. No truck obstacles added.\n";
    }
    else {
        // We will spawn 3 to 5 trucks randomly
        int numTrucks = (std::rand() % 6) + 5; // Generates a random number 3, 4, or 5

        // Define boundaries to keep trucks away from the edges
        const float padding = 80.f;
        const float max_x = (float)param::game_width - padding;
        const float min_x = padding;
        const float max_y = (float)param::game_height - padding;
        const float min_y = padding;

        for (int i = 0; i < numTrucks; ++i)
        {
            float randomX = randFloat(min_x, max_x);
            float randomY = randFloat(min_y, max_y);

            createTruckObstacle({ randomX, randomY });
        }
    }
    // ------------------------------------------

    // Initialize player
    g_player = std::make_unique<Player>(100.f, 100.f, g_playerTex);

    // Reset game state variables
    g_zombies.clear();
    g_zombieSpawnTimer = 0.f;
    g_difficultyTimer = 0.f;
    g_zombiesPerWave = 3;
    g_damageCooldown = 0.f;

    g_score = 0;
    g_isGameOver = false;

    // Initial zombie spawn
    spawnZombies(g_zombiesPerWave);
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

    // Player vs. Map Boundary (Clamping)
    sf::FloatRect pBounds = g_player->getBounds();
    float halfWidth = pBounds.width / 2.f;
    float halfHeight = pBounds.height / 2.f;

    sf::Vector2f pos = g_player->getCenter();

    // Clamp X position
    if (pos.x < halfWidth)
        pos.x = halfWidth;
    else if (pos.x > param::game_width - halfWidth)
        pos.x = param::game_width - halfWidth;

    // Clamp Y position
    if (pos.y < halfHeight)
        pos.y = halfHeight;
    else if (pos.y > param::game_height - halfHeight)
        pos.y = param::game_height - halfHeight;

    // Set the clamped position
    g_player->setPosition(pos);

    // Player vs. walls (includes the transparent truck collision box)
    // Recalculate pBounds since position might have been clamped
    pBounds = g_player->getBounds();
    for (auto& w : g_walls)
    {
        if (pBounds.intersects(w.getGlobalBounds()))
        {
            // If collision occurs, move player back by the difference
            g_player->moveBy(prevPos - g_player->getCenter());
            break;
        }
    }

    // Update zombies
    sf::Vector2f p = g_player->getCenter();
    for (auto& z : g_zombies)
        z.update(p, dt, g_walls); // This calls the reverted wall-aware update

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

        // Check for collision with walls (trucks) first
        bool hitWall = false;
        for (auto& w : g_walls)
        {
            if (b.getBounds().intersects(w.getGlobalBounds()))
            {
                b.kill();
                hitWall = true;
                break;
            }
        }
        if (hitWall) continue; // Bullet hit a wall, skip zombie check and move to next bullet

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

    // Player died ? GameOver
    if (g_player->getHP() <= 0)
    {
        g_isGameOver = true;
    }
}

void GameSystem::render(sf::RenderWindow& window)
{
    // 1. Draw Map Background
    if (g_mapTex.getSize().x > 0) {
        window.draw(g_mapSprite);
    }
    else {
        // Fallback: Clear with a color if map.png failed to load
        window.clear(sf::Color(25, 25, 40));
    }

    // 2. Draw Obstacles (Transparent collision boxes + Opaque Truck Sprites)
    // Draw the transparent collision boxes first for Z-order consistency
    for (auto& w : g_walls)
        window.draw(w);

    // Draw all the visual truck sprites
    for (const auto& truckSprite : g_truckSprites) {
        window.draw(truckSprite);
    }

    // 3. Draw Game Objects
    for (auto& z : g_zombies)
        z.draw(window);

    g_player->draw(window);

    // 4. Draw HUD
    sf::Font hudFont;
    if (!hudFont.loadFromFile("assets/arial.ttf")) {
        // Handle font loading error here if needed
    }


    sf::Text hp("HP: " + std::to_string(g_player->getHP()), hudFont, 18);
    hp.setFillColor(sf::Color::White);
    hp.setPosition(10, 10);
    window.draw(hp);

    sf::Text sc("Score: " + std::to_string(g_score), hudFont, 18);
    sc.setFillColor(sf::Color::Cyan);
    sc.setPosition(param::game_width - 120, 10);
    window.draw(sc);
}