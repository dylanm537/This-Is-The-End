#include "game_system.hpp"
#include "game_parameters.hpp"
#include "Player.hpp"
#include "Zombie.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm> 
#include <random> 

using param = Parameters;


sf::Texture GameSystem::spritesheet;


namespace
{
   
    static sf::Texture g_playerTex;

    static sf::Texture g_zombieTex;

    
    static sf::Texture g_mapTex;

    static sf::Sprite g_mapSprite;

    
    static sf::Texture g_truckTex;

    static std::vector<sf::Sprite> g_truckSprites;

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


static float randFloat(float a, float b) {
    
    static std::mt19937 rng((unsigned)std::random_device{}());

    std::uniform_real_distribution<float> d(a, b);
    return d(rng);
}


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
    
    const float offset = 40.f;

    for (int i = 0; i < count; i++)
    {
        float x, y;

        
        int side = (std::rand() % 4) + 1;

        if (side == 1)
        {
            
            x = randFloat(0.f, (float)param::game_width);

            y = -offset;
        }
        else if (side == 2) 
        {
            
            x = randFloat(0.f, (float)param::game_width);

            y = (float)param::game_height + offset;
        }
        else if (side == 3) 
        {
            
            x = -offset;

            y = randFloat(0.f, (float)param::game_height);
        }
        else 
        {
            
            x = (float)param::game_width + offset;

            y = randFloat(0.f, (float)param::game_height);
        }

        g_zombies.emplace_back(x, y, g_zombieTex);
    }
}



static void createTruckObstacle(sf::Vector2f position)
{
    
    const float desiredWidth = 120.f;

    const float collisionWidth = 28.f;

    const float collisionHeight = 38.f;

  
    sf::Sprite truckSprite(g_truckTex);

    sf::FloatRect bounds = truckSprite.getLocalBounds();

    float scale = desiredWidth / bounds.width;

    truckSprite.setScale(scale, scale);
    truckSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    truckSprite.setPosition(position);

    g_truckSprites.push_back(truckSprite);

    sf::RectangleShape truckCollision({ collisionWidth, collisionHeight });

    
    truckCollision.setOrigin(collisionWidth / 2.f, collisionHeight / 2.f);
    truckCollision.setPosition(position);

    
    truckCollision.setFillColor(sf::Color::Transparent);

    g_walls.push_back(truckCollision);
}


void GameSystem::init()
{
   
    if (!g_playerTex.loadFromFile("assets/player.png")) {
        std::cerr << "cant load assets/player.png\n";
    }

    if (!g_zombieTex.loadFromFile("assets/zombie.png")) {
        std::cerr << "cant load assets/zombie.png\n";
    }

    
    if (!g_mapTex.loadFromFile("assets/map.png")) {
        std::cerr << "cant load assets/map.png. will use default background.\n";
    }
    else {
        g_mapSprite.setTexture(g_mapTex);

        sf::Vector2u mapSize = g_mapTex.getSize();

        float scaleX = (float)param::game_width / mapSize.x;

        float scaleY = (float)param::game_height / mapSize.y;

        g_mapSprite.setScale(scaleX, scaleY);
    }

    g_walls.clear(); 

    g_truckSprites.clear(); 

    if (!g_truckTex.loadFromFile("assets/truck.png")) {
        std::cerr << "cant load assets/truck.png. No truck added.\n";
    }
    else {
        
        int numTrucks = (std::rand() % 6) + 5; 

       
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
   

    
    g_player = std::make_unique<Player>(100.f, 100.f, g_playerTex);

    
    g_zombies.clear();

    g_zombieSpawnTimer = 0.f;

    g_difficultyTimer = 0.f;

    g_zombiesPerWave = 3;

    g_damageCooldown = 0.f;

    g_score = 0;
    g_isGameOver = false;

   
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

    
    sf::FloatRect pBounds = g_player->getBounds();

    float halfWidth = pBounds.width / 2.f;
    float halfHeight = pBounds.height / 2.f;

    sf::Vector2f pos = g_player->getCenter();

   
    if (pos.x < halfWidth)
        pos.x = halfWidth;
    else if (pos.x > param::game_width - halfWidth)
        pos.x = param::game_width - halfWidth;

    
    if (pos.y < halfHeight)
        pos.y = halfHeight;
    else if (pos.y > param::game_height - halfHeight)
        pos.y = param::game_height - halfHeight;

    
    g_player->setPosition(pos);

    
    pBounds = g_player->getBounds();
    for (auto& w : g_walls)
    {
        if (pBounds.intersects(w.getGlobalBounds()))
        {
            
            g_player->moveBy(prevPos - g_player->getCenter());
            break;
        }
    }

  
    sf::Vector2f p = g_player->getCenter();

    for (auto& z : g_zombies)
        z.update(p, dt, g_walls); 

    
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

    
    for (auto& b : g_player->getBullets())
    {
        if (!b.isAlive()) continue;

        
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
        if (hitWall) continue; 

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

    
    g_zombies.erase(
        std::remove_if(g_zombies.begin(), g_zombies.end(),
            [](const Zombie& z) { return z.isDead(); }),
        g_zombies.end()
    );

    
    g_difficultyTimer += dt;
    if (g_difficultyTimer >= g_difficultyIncreaseInterval)
    {
        g_difficultyTimer = 0.f;

        g_zombiesPerWave += 3;
    }

    
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
    
    if (g_mapTex.getSize().x > 0) {
        window.draw(g_mapSprite);
    }
    else {
        
        window.clear(sf::Color(25, 25, 40));
    }

    
    for (auto& w : g_walls)
        window.draw(w);

    
    for (const auto& truckSprite : g_truckSprites) {
        window.draw(truckSprite);
    }

    
    for (auto& z : g_zombies)
        z.draw(window);

    g_player->draw(window);

    
    sf::Font hudFont;
    if (!hudFont.loadFromFile("assets/arial.ttf")) {
        
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