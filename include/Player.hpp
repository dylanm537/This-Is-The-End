#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

struct Bullet {
    sf::CircleShape shape;

    sf::Vector2f velocity;
    bool alive = true;

    void kill() { alive = false; }

    bool isAlive() const { return alive; }

    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }
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

    void setPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }

    void takeDamage(int amount) { hp -= amount; if (hp < 0) hp = 0; }

    int getHP() const { return hp; }

    std::vector<Bullet>& getBullets() { return bullets; }

    void moveBy(const sf::Vector2f& offset) { sprite.move(offset); }

private:
    sf::Sprite sprite;
    float speed;

    float bulletCooldown;

    float timeSinceLastShot;
    int hp;
    std::vector<Bullet> bullets;

    static constexpr int MAX_HP = 10;
};