// Zombie.h
#pragma once
#include <SFML/Graphics.hpp>

class Zombie {
public:
    Zombie(float x, float y, sf::Texture& texture);

    void update(const sf::Vector2f& playerPos, float dt);
    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    void damage(int amount) { hp -= amount; }
    int getHP() const { return hp; }
    bool isDead() const { return hp <= 0; }

    void moveBy(const sf::Vector2f& offset) { sprite.move(offset); }

private:
    sf::Sprite sprite;
    float speed;
    int hp;
    static constexpr int MAX_HP = 3;
};
