#include <SFML/Graphics.hpp>

int main() {
	sf::RenderWindow window(sf::VideoMode({ 540, 960 }), "EvolutionTD");
	sf::CircleShape shape(50.f);
	shape.setPosition({ 245.f, 455.f });
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen()) {
		while (auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}