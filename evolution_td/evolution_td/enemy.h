#pragma once
#include "asset_manager.h"
#include <SFML/Graphics.hpp>

class Enemy {
private:
	std::string name;
	sf::Vector2f virtualPos;
	int hp;
	int damage;
	float speed;
	float radius;
	std::size_t currentPathIndex = 0;

	sf::Sprite sprite;
	float texScale = 1.f;

public:
	Enemy(const std::string& _name, int _hp = 100, int _damage = 10, float _speed = 50.f, float _radius = 20.f, const sf::Texture& texture = sf::Texture()) : name(_name), hp(_hp), damage(_damage), speed(_speed), radius(_radius), sprite(texture) {
		sf::Vector2f texSize = sf::Vector2f(texture.getSize());
		sprite.setOrigin(texSize / 2.f);

		float targetWidth = radius * 2.f;
		texScale = targetWidth / texSize.x;
	}

	Enemy(const sf::Vector2f& _virtualPos, const std::string& _name, int _hp = 100, int _damage = 10, float _speed = 50.f, float _radius = 20.f, const sf::Texture& texture = sf::Texture()) : Enemy(_name, _hp, _damage, _speed, _radius, texture) {
		virtualPos = _virtualPos;
	}

	~Enemy() = default;

	void draw(sf::RenderWindow& window, float scale, float offsetX) {
		float x = virtualPos.x * scale + offsetX;
		float y = virtualPos.y * scale;

		sprite.setPosition({ x, y });
		sprite.setScale({ scale * texScale, scale * texScale });

		window.draw(sprite);
	}

	void move(float deltaTime, const std::vector<sf::Vector2f>& pathPoints) {
		if (currentPathIndex >= pathPoints.size()) {
			return;
		}

		sf::Vector2f target = pathPoints[currentPathIndex];
		sf::Vector2f direction = target - virtualPos;
		float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		float moveDistance = speed * deltaTime;

		if (distance <= moveDistance) {
			virtualPos = target;
			++currentPathIndex;
			std::cout << "Enemy " << name << " reached path point " << currentPathIndex << ", at " << virtualPos.x << ", " << virtualPos.y << "\n";
		}
		else {
			sf::Vector2f normDirection = direction / distance;
			virtualPos += normDirection * moveDistance;
		}
	}

	float getRadius() const {
		return radius;
	}
	
	std::string getName() const {
		return name;
	}

	void setVirtualPos(const sf::Vector2f& pos) {
		virtualPos = pos;
	}

	sf::Vector2f getVirtualPos() const {
		return virtualPos;
	}

	int getDamage() const {
		return damage;
	}

	bool hasReachedEnd(std::size_t pathSize) const {
		return currentPathIndex >= pathSize;
	}
};