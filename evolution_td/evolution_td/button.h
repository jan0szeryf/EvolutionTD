#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
private:
	sf::RectangleShape shape;
	sf::Text label;
	std::function<void()> onClick;

public:
    Button(sf::Vector2f size, sf::Vector2f pos, sf::Color color, const sf::Font& font, std::string text, std::function<void()> callback) : label(font, text), onClick(callback) {
        shape.setSize(size);
        shape.setPosition(pos);
        shape.setFillColor(color);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::White);

        sf::FloatRect textBounds = label.getLocalBounds();
        label.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.f,
            textBounds.position.y + textBounds.size.y / 2.f
            });

        label.setPosition({ pos.x + size.x / 2.f, pos.y + size.y / 2.f });
    }

	void draw(sf::RenderWindow& window) {
		window.draw(shape);
		window.draw(label);
	}

	bool isClicked(sf::Vector2i mousePos) {
		return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
	}

	void execute() {
		if (onClick) {
			onClick();
		}
	}

	void setPosition(sf::Vector2f pos) {
		shape.setPosition(pos);
		sf::FloatRect textBounds = label.getLocalBounds();
		label.setOrigin({
			textBounds.position.x + textBounds.size.x / 2.f,
			textBounds.position.y + textBounds.size.y / 2.f
			});
		label.setPosition({ pos.x + shape.getSize().x / 2.f, pos.y + shape.getSize().y / 2.f });
	}
};