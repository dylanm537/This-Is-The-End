// Player.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Bullet {
    sf::CircleShape shape;
    sf::Vector2f velocity;
};

class Player {
public:
    Player(float x, float y, sf::Texture& texture);

    void handleInput(const sf::RenderWindow& window, float dt);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    void shootTowards(const sf::Vector2f& target);
    void updateBullets(float dt);

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    sf::Vector2f getCenter() const { return sprite.getPosition(); }

    void takeDamage(int amount) { hp -= amount; if (hp < 0) hp = 0; }
    int getHP() const { return hp; }

    std::vector<Bullet>& getBullets() { return bullets; }

    // allow small external pushback for collision handling
    void moveBy(const sf::Vector2f& offset) { sprite.move(offset); }

private:
    sf::Sprite sprite;
    float speed;
    std::vector<Bullet> bullets;
    float bulletCooldown; // seconds
    float timeSinceLastShot;
    int hp;
    static constexpr int MAX_HP = 10;
};
