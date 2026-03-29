#pragma once
#include <concepts>
#include <SFML/Graphics.hpp>

template<typename T>
concept GameEntity = requires(T a, sf::Vector2u windowSize, sf::RenderWindow& window) {
	{ a.updateLayout(windowSize) } -> std::same_as<void>;
	{ a.draw(window) } -> std::same_as<void>;
};