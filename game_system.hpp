#pragma once
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

struct GameSystem {
    static sf::Texture spritesheet;

    static void init();
    static void reset();
    static void clean();
    static void update(const float& dt);
    static void render(sf::RenderWindow& window);
};