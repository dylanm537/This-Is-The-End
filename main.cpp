#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "game_parameters.hpp"
#include "game_system.hpp"

using gs = GameSystem;
using param = Parameters;


int main() {

	sf::RenderWindow window(sf::VideoMode({ param::game_width, param::game_height }), "This Is The End?");
	
	gs::init();

	while (window.isOpen()) {
		static sf::Clock clock;

		float dt = clock.restart().asSeconds();
		
		sf::Event event;
		
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return 0;
			}
		}

		window.clear();

		gs::update(dt);
		gs::render(window);
		
		sf::sleep(sf::seconds(param::time_step));

		window.display();
	}
	gs::clean();
	return 0;
}
