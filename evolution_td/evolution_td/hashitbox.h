#pragma once
#include <concepts>
#include <SFML/Graphics.hpp>

template<typename T>
concept HasHitbox = requires(T obj) {
	{ obj.getVirtualPos() } -> std::same_as<sf::Vector2f>;
	{ obj.getRadius() } -> std::same_as<float>;
};

template<typename T>
concept HasRange = requires(T obj) {
	{ obj.getVirtualPos() } -> std::same_as<sf::Vector2f>;
	{ obj.getRange() } -> std::same_as<float>;
};

template<HasHitbox A, HasHitbox B>
bool checkCollision(const A& a, const B& b) {
	sf::Vector2f dist = a.getVirtualPos() - b.getVirtualPos();
	float minDist = a.getRadius() + b.getRadius();
	return (dist.x * dist.x + dist.y * dist.y) <= (minDist * minDist);
}

template<HasRange tower, HasHitbox enemy>
bool checkInRange(const tower& t, const enemy& e) {
	sf::Vector2f dist = t.getVirtualPos() - e.getVirtualPos();
	float range = t.getRange();
	return (dist.x * dist.x + dist.y * dist.y) <= (range * range);
}