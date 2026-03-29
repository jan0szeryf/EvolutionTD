#include <SFML/Graphics.hpp>
#include "asset_manager.h"
#include "background.h"

int main() {
	AssetManager assetManager;
	assetManager.loadTextures("./assets/textures");

	sf::RenderWindow window(sf::VideoMode({ 540, 960 }), "EvolutionTD");

	Background background(assetManager.getTexture("forest1"));

	background.updateLayout(window.getSize());

	while (window.isOpen()) {
		while (auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (auto resized = event->getIf<sf::Event::Resized>()) {
				sf::FloatRect visibleArea({ 0, 0 }, { static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });
				window.setView(sf::View(visibleArea));

				background.updateLayout(resized->size);
			}
		}

		window.clear();

		background.draw(window);

		window.display();
	}

	return 0;
}