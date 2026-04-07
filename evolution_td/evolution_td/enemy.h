#pragma once
#include "asset_manager.h"
#include <SFML/Graphics.hpp>

class Enemy {
private:
	int hp;
	float speed;
	float radius;
	sf::Sprite sprite;

public:
	void draw(sf::RenderTarget& target) const {
		target.draw(sprite);
	}

	void updateLayout(float scale, float offsetX) {
		sprite.setScale({ scale, scale });
		sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
		sprite.setPosition({ offsetX + radius * scale, 0 });
	}

	void move(float deltaTime) {
		sprite.move({ speed * deltaTime, 0 });
	}
};