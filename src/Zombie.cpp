<<<<<<< Updated upstream
// Zombie.cpp
#include "Zombie.hpp"
=======
#include "Zombie.hpp"
#include "game_parameters.hpp"

>>>>>>> Stashed changes
#include <cmath>
#include <cstdlib> // std::rand

Zombie::Zombie()
    : m_speed(0.f)
    , m_alive(false)
{
    m_body.setRadius(GameParameters::ZombieRadius);
    m_body.setOrigin(GameParameters::ZombieRadius, GameParameters::ZombieRadius);
    m_body.setFillColor(sf::Color::Green);
}

// Always spawn just off the RIGHT side of the screen,
// at a random Y position. We ignore the passed-in position.
void Zombie::spawn(const sf::Vector2f& /*position*/, float speed)
{
    float y = static_cast<float>(std::rand() % GameParameters::WindowHeight);
    float x = GameParameters::WindowWidth + GameParameters::ZombieRadius * 2.f;

    m_body.setPosition(x, y);
    m_speed = speed;
    m_alive = true;

    // Face left
    m_body.setRotation(180.f);
}

// Move straight LEFT across the screen, ignoring targetPos.
void Zombie::update(float dt, const sf::Vector2f& /*targetPos*/)
{
    if (!m_alive)
        return;

    sf::Vector2f pos = m_body.getPosition();
    pos.x -= m_speed * dt;
    m_body.setPosition(pos);

    // If the zombie goes completely off the left side, kill it
    if (pos.x < -GameParameters::ZombieRadius * 2.f)
    {
        m_alive = false;
    }
}

void Zombie::draw(sf::RenderWindow& window) const
{
    if (m_alive)
        window.draw(m_body);
}

const sf::CircleShape& Zombie::getShape() const
{
    return m_body;
}

bool Zombie::isAlive() const
{
    return m_alive;
}

void Zombie::kill()
{
    m_alive = false;
}
