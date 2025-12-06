#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Zombie {
public:
    Zombie(float x, float y, sf::Texture& texture);

    // Wall-aware update (Original logic: simple collision/bounce)
    void update(const sf::Vector2f& playerPos, float dt,
        const std::vector<sf::RectangleShape>& walls);

    // Basic update (for compatibility, not used by GameSystem but must be defined)
    void update(const sf::Vector2f& playerPos, float dt);

    void draw(sf::RenderWindow& window) const;

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

    void damage(int amount) { hp -= amount; }
    bool isDead() const { return hp <= 0; }

private:
    sf::Sprite sprite;
    float speed;
    int hp;

    static constexpr int MAX_HP = 3;
};