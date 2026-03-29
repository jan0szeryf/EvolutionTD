#pragma once
#include <SFML/Graphics.hpp>

class Background {
private:
	sf::Sprite sprite;
	sf::Vector2f originalSize;

public:
	Background(const sf::Texture& texture) : sprite(texture) {
		originalSize = sf::Vector2f(texture.getSize());
		sprite.setOrigin(originalSize / 2.f);
	}

	void updateLayout(const sf::Vector2u& windowSize) {
		float scale = static_cast<float>(windowSize.y) / originalSize.y;
		sprite.setScale({ scale, scale });
		sprite.setPosition({ windowSize.x / 2.f, windowSize.y / 2.f });
	}

	void draw(sf::RenderWindow& window) const {
		window.draw(sprite);
	}
};