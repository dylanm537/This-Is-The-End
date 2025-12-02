#include "Player.hpp"
#include "game_parameters.hpp"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>

Player::Player()
    : m_lives(GameParameters::PlayerMaxLives)
    , m_invulnTimer(0.f)
{
    m_body.setRadius(GameParameters::PlayerRadius);
    m_body.setOrigin(GameParameters::PlayerRadius, GameParameters::PlayerRadius);
    m_body.setFillColor(sf::Color::Cyan);
    reset();
}

void Player::reset()
{
    m_lives = GameParameters::PlayerMaxLives;
    m_invulnTimer = 0.f;
    m_body.setPosition(
        GameParameters::WindowWidth * 0.5f,
        GameParameters::WindowHeight * 0.5f);
}

void Player::handleEvent(const sf::Event& /*event*/)
{
    // nothing event-based for now
}

void Player::update(float dt, const sf::RenderWindow& window)
{
    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;

    if (movement.x != 0.f || movement.y != 0.f)
    {
        float len = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        if (len != 0.f)
        {
            movement.x /= len;
            movement.y /= len;
        }
        movement.x *= GameParameters::PlayerSpeed * dt;
        movement.y *= GameParameters::PlayerSpeed * dt;

        m_body.move(movement);
    }

    // Keep within window bounds
    sf::Vector2f pos = m_body.getPosition();
    if (pos.x < GameParameters::PlayerRadius) pos.x = GameParameters::PlayerRadius;
    if (pos.y < GameParameters::PlayerRadius) pos.y = GameParameters::PlayerRadius;
    if (pos.x > GameParameters::WindowWidth - GameParameters::PlayerRadius) pos.x = GameParameters::WindowWidth - GameParameters::PlayerRadius;
    if (pos.y > GameParameters::WindowHeight - GameParameters::PlayerRadius) pos.y = GameParameters::WindowHeight - GameParameters::PlayerRadius;
    m_body.setPosition(pos);

    // Rotate towards mouse
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    sf::Vector2f dir = worldPos - m_body.getPosition();
    float angle = std::atan2(dir.y, dir.x) * 180.f / GameParameters::Pi;
    m_body.setRotation(angle + 90.f);
}

void Player::draw(sf::RenderWindow& window) const
{
    sf::CircleShape copy = m_body;

    // Flash when invulnerable
    if (m_invulnTimer > 0.f)
    {
        copy.setFillColor(sf::Color(255, 255, 255, 150));
    }

    window.draw(copy);
}

sf::Vector2f Player::getPosition() const
{
    return m_body.getPosition();
}

const sf::CircleShape& Player::getShape() const
{
    return m_body;
}

void Player::damage()
{
    if (m_invulnTimer <= 0.f && m_lives > 0)
    {
        --m_lives;
        m_invulnTimer = GameParameters::PlayerInvulnTime;
    }
}

bool Player::isDead() const
{
    return m_lives <= 0;
}

int Player::getLives() const
{
    return m_lives;
}

void Player::updateInvulnerability(float dt)
{
    if (m_invulnTimer > 0.f)
    {
        m_invulnTimer -= dt;
        if (m_invulnTimer < 0.f)
            m_invulnTimer = 0.f;
    }
}

bool Player::isInvulnerable() const
{
    return m_invulnTimer > 0.f;
}
