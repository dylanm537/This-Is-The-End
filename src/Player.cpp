#include "Player.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm> // Ensure this is present for std::remove_if to work

namespace
{
    sf::Vector2f normalize(const sf::Vector2f& v)
    {
        float lenSq = v.x * v.x + v.y * v.y;
        if (lenSq == 0.f) return { 0.f, 0.f };
        float invLen = 1.f / std::sqrt(lenSq);
        return { v.x * invLen, v.y * invLen };
    }
}

Player::Player(float x, float y, sf::Texture& texture)
    : speed(220.f)
    , bulletCooldown(0.18f)
    , timeSinceLastShot(0.f)
    , hp(MAX_HP)
{
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();

    // Set origin to the center of the sprite image
    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

    // Scale the sprite to a reasonable size (e.g., 30 pixels high)
    const float desiredHeight = 30.f;
    float scale = desiredHeight / bounds.height;
    sprite.setScale(scale, scale);

    sprite.setPosition(x, y);
}

void Player::handleInput(const sf::RenderWindow& window, float dt)
{
    sf::Vector2f dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1.f;

    // Normalize direction and move
    dir = normalize(dir);
    sprite.move(dir * speed * dt);

    // Calculate rotation angle
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos);

    sf::Vector2f lookDir = mouseWorld - sprite.getPosition();
    float angle = std::atan2(lookDir.y, lookDir.x) * 180.f / 3.14159265f;

    // ?? FIX 1: Removed the + 90.f offset because the sprite is oriented RIGHT (0 degrees)
    sprite.setRotation(angle); // Now the sprite faces the mouse correctly

    // Shooting
    timeSinceLastShot += dt;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (timeSinceLastShot >= bulletCooldown)
        {
            shootTowards(mouseWorld);
            timeSinceLastShot = 0.f;
        }
    }
}

void Player::update(float /*dt*/)
{
    // Player body
}

void Player::draw(sf::RenderWindow& window) const
{
    // Draw bullets
    for (const auto& b : bullets)
        if (b.alive)
            window.draw(b.shape);

    // Draw player
    window.draw(sprite);
}

void Player::shootTowards(const sf::Vector2f& target)
{
    Bullet b;
    b.shape = sf::CircleShape(4.f);
    b.shape.setOrigin(4.f, 4.f);
    b.shape.setFillColor(sf::Color::Yellow);

    const float bulletSpeed = 460.f;
    sf::Vector2f dir = normalize(target - sprite.getPosition());

    // ?? FIX 2: Offset the bullet spawn point forward along the direction vector
    const float spawnOffset = 20.f; // Adjust this value to fine-tune the starting position
    sf::Vector2f spawnPos = sprite.getPosition() + dir * spawnOffset;

    b.shape.setPosition(spawnPos); // Spawn the bullet in front of the center

    b.velocity = dir * bulletSpeed;

    bullets.push_back(b);
}

void Player::updateBullets(float dt)
{
    // Move bullets
    for (auto& b : bullets)
    {
        if (b.alive)
            b.shape.move(b.velocity * dt);
    }

    // Cull expired bullets (simple screen boundary check)
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) {
                // Assuming game dimensions are 800x600 for culling logic
                return !b.alive || b.shape.getPosition().x < 0 || b.shape.getPosition().x > 800 ||
                    b.shape.getPosition().y < 0 || b.shape.getPosition().y > 600;
            }),
        bullets.end());
}