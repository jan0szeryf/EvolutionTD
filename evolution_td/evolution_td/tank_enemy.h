#pragma once
#include "enemy.h"

class TankEnemy : public Enemy {
private:

public:
	TankEnemy(const std::string& _name = "TankEnemy", int _hp = 200, int _damage = 5, int _reward = 50, float _speed = 30.f, float _radius = 25.f, const sf::Texture& texture = sf::Texture())
		: Enemy(_name, _hp, _damage, _reward, _speed, _radius, texture) {}

	TankEnemy(const sf::Vector2f& _virtualPos, const std::string& _name = "TankEnemy", int _hp = 200, int _damage = 5, int _reward = 50, float _speed = 30.f, float _radius = 25.f, const sf::Texture& texture = sf::Texture()) : Enemy(_virtualPos, _name, _hp, _damage, _reward, _speed, _radius, texture) {}
};