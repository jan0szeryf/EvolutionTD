#pragma once
#include <concepts>
#include <SFML/Graphics.hpp>

template<typename T>
concept HasHitbox = requires(T obj) {
	{ obj.getVirtualPos() } -> std::same_as<sf::Vector2f>;
	{ obj.getRadius() } -> std::same_as<float>;
};

template<HasHitbox A, HasHitbox B>
bool checkCollision(const A& a, const B& b) {
	sf::Vector2f dist = a.getVirtualPos() - b.getVirtualPos();
	float minDist = a.getRadius() + b.getRadius();
	return (dist.x * dist.x + dist.y * dist.y) <= (minDist * minDist);
}