// Zombie.cpp
#include "Zombie.hpp"
#include <cmath>
#include <random>

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
    sprite.setOrigin(b.width/2.f, b.height/2.f);
    sprite.setPosition(x, y);

    float desired = 30.f;
    float scale = desired / std::max(b.width, b.height);
    sprite.setScale(scale, scale);
}

void Zombie::update(const sf::Vector2f& playerPos, float dt) {
    sf::Vector2f dir = playerPos - sprite.getPosition();
    float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
    if (len == 0.f) return;
    dir /= len;
    sprite.move(dir * speed * dt);
}

void Zombie::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
    // draw HP bar
    sf::RectangleShape back({30.f, 4.f});
    back.setFillColor({150,150,150});
    back.setPosition(sprite.getPosition().x - 15.f, sprite.getPosition().y - 22.f);
    window.draw(back);
    float w = 30.f * (float(hp) / MAX_HP);
    sf::RectangleShape hpBar({w, 4.f});
    hpBar.setFillColor({0,200,0});
    hpBar.setPosition(back.getPosition());
    window.draw(hpBar);
}
