#include "Player.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

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

    sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    sprite.setPosition(x, y);
}

void Player::handleInput(const sf::RenderWindow& window, float dt)
{
    sf::Vector2f dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) dir.x += 1.f;

    dir = normalize(dir);
    sprite.move(dir * speed * dt);

    // Aim at mouse
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);
    sf::Vector2f toMouse = mouseWorld - sprite.getPosition();

    float angle = std::atan2(toMouse.y, toMouse.x) * 180.f / 3.14159265f;
    sprite.setRotation(angle + 90.f);

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
    b.shape.setPosition(sprite.getPosition());

    const float bulletSpeed = 460.f;
    sf::Vector2f dir = normalize(target - sprite.getPosition());
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

    // Remove dead or out-of-bounds bullets
    auto it = bullets.begin();
    while (it != bullets.end())
    {
        sf::Vector2f p = it->shape.getPosition();
        bool outOfBounds =
            (p.x < -100.f || p.x > 1000.f ||
                p.y < -100.f || p.y > 800.f);

        if (!it->alive || outOfBounds)
            it = bullets.erase(it);
        else
            ++it;
    }
}
