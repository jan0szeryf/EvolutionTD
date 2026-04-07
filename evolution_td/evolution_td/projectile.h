#pragma once
#include "asset_manager.h"
#include "enemy.h"
#include <SFML/Graphics.hpp>

class Projectile {
private:
	float speed;
	float damage;
	std::shared_ptr<Enemy> target;
	sf::Sprite sprite;

public:
	Projectile(float _speed, float _damage, const sf::Texture& texture) : speed(_speed), damage(_damage), sprite(texture) {
		sf::Vector2f texSize = sf::Vector2f(texture.getSize());
		sprite.setOrigin(texSize / 2.f);
	}
};