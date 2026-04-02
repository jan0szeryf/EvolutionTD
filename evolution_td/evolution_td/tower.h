#pragma once
#include "asset_manager.h"
#include <SFML/Graphics.hpp>

class Tower {
private:
	sf::Vector2f virtualPos;
	int damage;
	float range;
	float fireRate;
	float radius;
	int cost;
	sf::Sprite sprite;
	sf::Sprite bulletSprite;

	float texScale = 1.f;

public:
	Tower(sf::Vector2f _virtualPos, int _damage, float _range, float _fireRate, float _radius, int _cost, const sf::Texture& texture, const sf::Texture& bulletTexture) : virtualPos(_virtualPos), damage(_damage), range(_range), fireRate(_fireRate), radius(_radius), cost(_cost), sprite(texture), bulletSprite(bulletTexture) {
		sf::Vector2f texSize = sf::Vector2f(texture.getSize());
		sprite.setOrigin(texSize / 2.f);

		float targetWidth = radius * 2.f;
		texScale = targetWidth / texSize.x;
	}

	float getRadius() const {
		return radius;
	}
	
	sf::Vector2f getVirtualPos() const {
		return virtualPos;
	}

	void draw(sf::RenderWindow& window, float scale, float offsetX) {
		float x = virtualPos.x * scale + offsetX;
		float y = virtualPos.y * scale;

		sprite.setPosition({ x, y });
		sprite.setScale({ scale*texScale, scale*texScale });

		window.draw(sprite);
	}

	void drawRange(sf::RenderWindow& window, float scale, float offsetX) const {
		float x = virtualPos.x * scale + offsetX;
		float y = virtualPos.y * scale;
		float scaledRange = range * scale;

		sf::CircleShape rangeCircle(scaledRange);
		rangeCircle.setOrigin({ scaledRange, scaledRange });
		rangeCircle.setPosition({ x, y });
		rangeCircle.setFillColor(sf::Color(255, 255, 255, 50));
		rangeCircle.setOutlineThickness(1.f);
		rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 100));
		window.draw(rangeCircle);
	}
	
	void drawRadius(sf::RenderWindow& window, float scale, float offsetX) const {
		float x = virtualPos.x * scale + offsetX;
		float y = virtualPos.y * scale;
		float scaledRadius = radius * scale;

		sf::CircleShape rangeCircle(scaledRadius);
		rangeCircle.setOrigin({ scaledRadius, scaledRadius });
		rangeCircle.setPosition({ x, y });
		rangeCircle.setFillColor(sf::Color(255, 255, 255, 50));
		rangeCircle.setOutlineThickness(1.f);
		rangeCircle.setOutlineColor(sf::Color(255, 255, 255, 100));
		window.draw(rangeCircle);
	}

	void setVirtualPos(sf::Vector2f newPos) {
		virtualPos = newPos;
	}

	void setColor(sf::Color color) {
		sprite.setColor(color);
	}
};