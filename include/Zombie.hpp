#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Zombie {
public:
    //creates a zombie at position (x, y) with texture
    Zombie(float x, float y, sf::Texture& texture);
    // chase the player 
    void update(const sf::Vector2f& playerPos, float dt,
        // chase player but also avoids walls 
        const std::vector<sf::RectangleShape>& walls);

    
    void update(const sf::Vector2f& playerPos, float dt);
    //draw sprite and hp bar if alive 
    void draw(sf::RenderWindow& window) const;
    // collision bounds 
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
    // damage and death handling
    void damage(int amount) { hp -= amount; }

    bool isDead() const { return hp <= 0; }

private:
    sf::Sprite sprite;
    float speed; // randomized per zombie for variation 

    int hp;

    static constexpr int MAX_HP = 3;
};