#pragma once
#include "enemy.h"

class FastEnemy : public Enemy {
private:

public:
	FastEnemy(const std::string& _name = "FastEnemy", int _hp = 50, int _damage = 5, float _speed = 100.f, float _radius = 15.f, const sf::Texture& texture = sf::Texture()) 
		: Enemy(_name, _hp, _damage, _speed, _radius, texture) {}

	FastEnemy(const sf::Vector2f& _virtualPos, const std::string& _name = "FastEnemy", int _hp = 50, int _damage = 5, float _speed = 100.f, float _radius = 15.f, const sf::Texture& texture = sf::Texture()) : Enemy(_virtualPos, _name, _hp, _damage, _speed, _radius, texture) {}
};