#include "Zombie.hpp"
#include <cmath>
#include <random>
#include <algorithm>

// random float helper
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


// BASIC UPDATE

void Zombie::update(const sf::Vector2f& playerPos, float dt)
{
    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = playerPos - pos;

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len <= 0.f) return;
    dir /= len;

    sprite.move(dir * speed * dt);

    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle + 90.f);
}


// WALL-AWARE 

void Zombie::update(const sf::Vector2f& playerPos, float dt,
    const std::vector<sf::RectangleShape>& walls)
{
    if (hp <= 0)
        return;

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = playerPos - pos;

    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len <= 0.f)
        return;

    dir /= len;

    // Predict next position
    sf::Vector2f nextPos = pos + dir * speed * dt;
    sf::FloatRect nextBounds = sprite.getGlobalBounds();
    nextBounds.left = nextPos.x - nextBounds.width * 0.5f;
    nextBounds.top = nextPos.y - nextBounds.height * 0.5f;

    bool blocked = false;

    for (const auto& w : walls)
    {
        if (nextBounds.intersects(w.getGlobalBounds()))
        {
            blocked = true;
            break;
        }
    }

    // if no collision then can move
    if (!blocked)
    {
        sprite.setPosition(nextPos);
    }
    else
    {
        // WALL AVOIDANCE
        sf::Vector2f leftNormal(-dir.y, dir.x);  // rotate 90°
        sf::Vector2f leftTry = pos + leftNormal * speed * dt;
        sf::Vector2f rightTry = pos - leftNormal * speed * dt;

        sf::FloatRect leftBounds = nextBounds;
        leftBounds.left = leftTry.x - leftBounds.width * 0.5f;
        leftBounds.top = leftTry.y - leftBounds.height * 0.5f;

        sf::FloatRect rightBounds = nextBounds;
        rightBounds.left = rightTry.x - rightBounds.width * 0.5f;
        rightBounds.top = rightTry.y - rightBounds.height * 0.5f;

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
        // else stuck dont move
    }

    // Rotate toward player
    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle + 90.f);
}


// DRAW

void Zombie::draw(sf::RenderWindow& window) const
{
    if (hp <= 0)
        return;

    window.draw(sprite);

    // HP bar
    sf::Vector2f p = sprite.getPosition();

    sf::RectangleShape back({ 30.f, 4.f });
    back.setFillColor({ 150,150,150 });
    back.setPosition(p.x - 15.f, p.y - 22.f);
    window.draw(back);

    float w = 30.f * (float(hp) / MAX_HP);
    sf::RectangleShape hpBar({ w, 4.f });
    hpBar.setFillColor({ 0,200,0 });
    hpBar.setPosition(back.getPosition());
    window.draw(hpBar);
}
