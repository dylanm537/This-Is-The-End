#include "Zombie.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>

static float randFloat(float a, float b) {
    static std::mt19937 rng((unsigned)std::random_device{}());
    std::uniform_real_distribution<float> d(a, b);
    return d(rng);
}

Zombie::Zombie(float x, float y, sf::Texture& texture)
    : speed(randFloat(40.f, 90.f)), hp(MAX_HP)
{
    sprite.setTexture(texture);

    sf::FloatRect b = sprite.getLocalBounds();
    sprite.setOrigin(b.width / 2.f, b.height / 2.f);
    sprite.setPosition(x, y);

    float desired = 30.f;
    float scale = desired / std::max(b.width, b.height);
    sprite.setScale(scale, scale);
}


void Zombie::update(const sf::Vector2f& playerPos, float dt)
{
    if (hp <= 0) return;

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = playerPos - pos;

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len <= 0.f) return;
    dir /= len;

    sprite.move(dir * speed * dt);

    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle + 90.f);
}


void Zombie::update(const sf::Vector2f& playerPos, float dt,
    const std::vector<sf::RectangleShape>& walls)
{
    if (hp <= 0) return;

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = playerPos - pos;

   
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len <= 0.f) return;
    dir /= len;


    sprite.move(dir * speed * dt);

  
    sf::FloatRect nextBounds = getBounds();
    bool collided = false;

    
    for (const auto& w : walls)
    {
        if (nextBounds.intersects(w.getGlobalBounds()))
        {
            collided = true;
            break;
        }
    }

    if (collided)
    {
       
        sprite.move(-dir * speed * dt);

        
        sf::Vector2f perp = { dir.y, -dir.x }; 

        
        sf::Vector2f currentPos = sprite.getPosition();
        sf::Vector2f leftTry = currentPos + perp * 2.f;
        sf::Vector2f rightTry = currentPos - perp * 2.f;

        
        sf::FloatRect bounds = getBounds();
        float halfWidth = bounds.width * 0.5f;
        float halfHeight = bounds.height * 0.5f;

        sf::FloatRect leftBounds = nextBounds;
        leftBounds.left = leftTry.x - halfWidth;
        leftBounds.top = leftTry.y - halfHeight;

        sf::FloatRect rightBounds = nextBounds;
        rightBounds.left = rightTry.x - halfWidth;
        rightBounds.top = rightTry.y - halfHeight;

        bool leftOK = true, rightOK = true;

        
        for (const auto& w : walls)
        {
            if (leftBounds.intersects(w.getGlobalBounds()))  leftOK = false;
            if (rightBounds.intersects(w.getGlobalBounds())) rightOK = false;
        }

        if (leftOK)
            sprite.setPosition(leftTry);
        else if (rightOK)
            sprite.setPosition(rightTry);
       
    }

    
    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle + 90.f);
}


void Zombie::draw(sf::RenderWindow& window) const
{
    if (hp <= 0)
        return;

    window.draw(sprite);

    
    sf::Vector2f p = sprite.getPosition();

    sf::RectangleShape back({ 30.f, 4.f });
    back.setOrigin(15.f, 2.f);
    back.setFillColor(sf::Color(100, 0, 0));
    back.setPosition(p.x, p.y - 30.f);
    window.draw(back);

    sf::RectangleShape front({ 30.f * ((float)hp / (float)MAX_HP), 4.f });
    front.setOrigin(15.f, 2.f);
    front.setFillColor(sf::Color(0, 200, 0));
    front.setPosition(p.x, p.y - 30.f);
    window.draw(front);
}