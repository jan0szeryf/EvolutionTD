#pragma once
#include "asset_manager.h"
#include "enemy.h"
#include <SFML/Graphics.hpp>
#include <cmath>

class Projectile {
private:
	sf::Vector2f virtualPos;
	sf::Vector2f targetPos;
	float speed;
	float damage;
	float radius;
	float texScale = 1.f;
	sf::Sprite sprite;

public:
	Projectile(sf::Vector2f _virtualPos, sf::Vector2f _targetPos, const sf::Texture& texture, float _damage) : virtualPos(_virtualPos), targetPos(_targetPos), sprite(texture), damage(_damage), radius(16.f), speed(500.f) {
		sf::Vector2f texSize = sf::Vector2f(texture.getSize());
		sprite.setOrigin(texSize / 2.f);

		float targetWidth = radius * 2.f;
		texScale = targetWidth / texSize.x;
	}

	sf::Vector2f getVirtualPos() const {
		return virtualPos;
	}

	float getRadius() const {
		return radius;
	}

	float getDamage() const {
		return damage;
	}

	sf::Vector2f getTargetPos() const {
		return targetPos;
	}

	void move(float deltaTime) {
		sf::Vector2f direction = targetPos - virtualPos;
		float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);

		if (dist > 0.1f) {
			float moveDist = speed * deltaTime;

			if (moveDist >= dist) {
				virtualPos = targetPos;
			}
			else {
				virtualPos += direction / dist * moveDist;
			}
		}
	}

	void draw(sf::RenderWindow& window, float scale, float offsetX) {
		float x = virtualPos.x * scale + offsetX;
		float y = virtualPos.y * scale;
		sprite.setPosition({ x, y });
		sprite.setScale({ scale * texScale, scale * texScale });
		window.draw(sprite);
	}
};