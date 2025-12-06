#include "Zombie.hpp"
#include <cmath>
#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>

// random float helper (kept from previous version)
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

// ---------------------------------------------------------------------------
// BASIC UPDATE (If only playerPos is provided)
// ---------------------------------------------------------------------------
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

// ---------------------------------------------------------------------------
// WALL-AWARE UPDATE (Reverted to simple collision response logic)
// ---------------------------------------------------------------------------
void Zombie::update(const sf::Vector2f& playerPos, float dt,
    const std::vector<sf::RectangleShape>& walls)
{
    if (hp <= 0) return;

    sf::Vector2f pos = sprite.getPosition();
    sf::Vector2f dir = playerPos - pos;

    // 1. Calculate direction to player and normalize it
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len <= 0.f) return;
    dir /= len;

    // 2. Move towards player (this might move into a wall)
    sprite.move(dir * speed * dt);

    // 3. Collision Resolution (simple bounce back, and attempt to move left/right)
    sf::FloatRect nextBounds = getBounds();
    bool collided = false;

    // Check if the current position is now in collision with any wall
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
        // If collided, move it back by one frame's movement
        sprite.move(-dir * speed * dt);

        // Then, try to move slightly left/right (perpendicular to direction of travel)
        sf::Vector2f perp = { dir.y, -dir.x }; // Left turn

        // Calculate trial positions
        sf::Vector2f currentPos = sprite.getPosition();
        sf::Vector2f leftTry = currentPos + perp * 2.f;
        sf::Vector2f rightTry = currentPos - perp * 2.f;

        // Calculate bounds for trial positions
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

        // Check if trial positions are free of walls
        for (const auto& w : walls)
        {
            if (leftBounds.intersects(w.getGlobalBounds()))  leftOK = false;
            if (rightBounds.intersects(w.getGlobalBounds())) rightOK = false;
        }

        // Apply the first clear movement found
        if (leftOK)
            sprite.setPosition(leftTry);
        else if (rightOK)
            sprite.setPosition(rightTry);
        // else: stuck, don't move until wall is no longer there
    }

    // 4. Update Rotation
    float angle = std::atan2(dir.y, dir.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle + 90.f);
}

// --------------------------------------------------------------------------
// DRAW
// --------------------------------------------------------------------------
void Zombie::draw(sf::RenderWindow& window) const
{
    if (hp <= 0)
        return;

    window.draw(sprite);

    // HP bar
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