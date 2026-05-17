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

	void draw(sf::RenderWindow& window, float scale, float offsetX) {
		float x = virtualPos.x * scale + offsetX;
		float y = virtualPos.y * scale;

		sprite.setPosition({ x, y });
		sprite.setScale({ scale * texScale, scale * texScale });

		window.draw(sprite);
	}

	void move(float deltaTime) {
		//todo: move along path
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
};