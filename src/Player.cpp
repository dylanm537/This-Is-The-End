#include "Player.hpp"
#include "game_parameters.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm> 

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

   
    dir = normalize(dir);
    sprite.move(dir * speed * dt);

    
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos);

    sf::Vector2f lookDir = mouseWorld - sprite.getPosition();
    float angle = std::atan2(lookDir.y, lookDir.x) * 180.f / 3.14159265f;

   
    sprite.setRotation(angle);

    
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
    
}

void Player::draw(sf::RenderWindow& window) const
{
    
    for (const auto& b : bullets)
        if (b.alive)
            window.draw(b.shape);

    
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

    
    const float spawnOffset = 20.f;
    sf::Vector2f spawnPos = sprite.getPosition() + dir * spawnOffset;

    b.shape.setPosition(spawnPos);

    b.velocity = dir * bulletSpeed;

    bullets.push_back(b);
}

void Player::updateBullets(float dt)
{
    
    for (auto& b : bullets)
    {
        if (b.alive)
            b.shape.move(b.velocity * dt);
    }

    
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) {
                
                return !b.alive ||
                    b.shape.getPosition().x < 0 ||
                    b.shape.getPosition().x > Parameters::game_width ||
                    b.shape.getPosition().y < 0 ||
                    b.shape.getPosition().y > Parameters::game_height;
            }),
        bullets.end());
}